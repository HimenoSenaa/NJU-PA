#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ, TK_NUM, TK_HEXNUM,
  TK_LPARE, TK_RPARE,TK_COMMA, TK_PLUS, TK_DEREF, 
  TK_DIVI, TK_MULTI, TK_REGNAME, TK_MINUS, TK_NEG,
  TK_AND, TK_OR, TK_LMOV, TK_RMOV, TK_BOR, TK_XOR, TK_BAND,
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", TK_PLUS},         // plus
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},
  {"&&", TK_AND},
  {"\\|\\|", TK_OR},
  {"0x[0-9,a-f]+", TK_HEXNUM},
  {"\\$(0|ra|sp|gp|tp|t0|t1|t2|s0|s1|a0|a1|a2|a3|a4|a5|a6|a7|s2|s3|s4|s5|s6|s7|s8|s9|s10|s11|t3|t4|t5|t6|pc)", TK_REGNAME},
  {"[0-9]+", TK_NUM},
  {"<<", TK_LMOV},
  {">>", TK_RMOV},
  {"\\|", TK_BOR},
  {"\\&", TK_AND},
  {"\\^", TK_XOR},
  {"\\-", TK_MINUS},
  {"\\*", TK_MULTI},
  {"\\/", TK_DIVI},
  {"\\(", TK_LPARE},
  {"\\)", TK_RPARE},
  {",", TK_COMMA},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (position < strlen(e) &&  e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_PLUS :
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_PLUS;
                  nr_token++;
                  break;
          case TK_MINUS :
                  //printf("%d\n", nr_token);
                  if(nr_token >= 1 && (tokens[nr_token-1].type == TK_REGNAME || tokens[nr_token-1].type == TK_NUM || tokens[nr_token-1].type == TK_RPARE))
                    tokens[nr_token].type = TK_MINUS ;
                  else tokens[nr_token].type = TK_NEG;
                  nr_token++;
                  break;
          case TK_MULTI :
                  //printf("%d\n", nr_token);
                  if(nr_token >= 1 && (tokens[nr_token-1].type == TK_REGNAME || tokens[nr_token-1].type == TK_NUM || tokens[nr_token-1].type == TK_RPARE))
                    tokens[nr_token].type = TK_MULTI;
                  else 
                    tokens[nr_token].type = TK_DEREF;
                  nr_token++;
                  break;
          case TK_DIVI :
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_DIVI;
                  nr_token++;
                  break;
          case TK_LPARE :
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_LPARE;
                  nr_token++;
                  break;
          case TK_RPARE :
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_RPARE;
                  nr_token++;
                  break;
          case TK_EQ : 
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_EQ;
                  nr_token++;
                  break;
          case TK_NEQ :
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_NEQ;
                  nr_token++;
                  break;
          case TK_REGNAME:
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_REGNAME;
                  strncpy(tokens[nr_token].str, substr_start+1, substr_len-1);  
                  // printf("%s\n", tokens[nr_token].str);
                  nr_token++;
                  break;
          case TK_AND : 
                  tokens[nr_token].type = TK_AND;
                  nr_token++;
                  break;
          case TK_OR  :
                  tokens[nr_token].type = TK_OR;
                  nr_token++;
                  break;
          case TK_LMOV :
                  tokens[nr_token].type = TK_LMOV;
                  nr_token++;
                  break;
          case TK_RMOV :
                  tokens[nr_token].type = TK_RMOV;
                  nr_token++;
                  break;     
          case TK_BAND :
                  tokens[nr_token].type = TK_BAND;
                  nr_token++;
                  break;   
          case TK_BOR :
                  tokens[nr_token].type = TK_BOR;
                  nr_token++;
                  break;  
          case TK_XOR :
                  tokens[nr_token].type = TK_XOR;
                  nr_token++;
                  break; 
          case TK_HEXNUM:
                  tokens[nr_token].type = TK_NUM;
                  if(substr_len < 32){
                         strncpy(tokens[nr_token].str, substr_start+2, substr_len-2);  
                         // printf("%s\n", tokens[nr_token].str);
                         int ans = 0;
                         for(int i = 0; i < strlen(tokens[nr_token].str); i++) {
                          if(tokens[nr_token].str[i] <= '9')
                            ans = ans * 16 + tokens[nr_token].str[i] - '0';
                          else
                            ans = ans * 16 + tokens[nr_token].str[i] - 'a' + 10;
                         }
                         char buf[32] = {};
                         memset(buf, 0, sizeof(buf));
                         sprintf(buf, "%d", ans);
                         strcpy(tokens[nr_token].str, buf);
                  } else {
                          printf("number too long\n");
                  }
                  nr_token++;
                  break;
          case TK_NUM :
                  //printf("%d\n", nr_token);
                  tokens[nr_token].type = TK_NUM;
                  if(substr_len < 32){
                         strncpy(tokens[nr_token].str, substr_start, substr_len);  
                         // printf("%s\n", tokens[nr_token].str);
                  } else {
                          printf("number too long\n");
                  }
                  nr_token++;
                  break;
        }
        // Log("%c  %s\n", tokens[nr_token-1].type, tokens[nr_token-1].str);
        break;
      }
    }

    if (i == NR_REGEX) {
      // printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static bool check_parentheses(int p, int q) {
  int cnt = 0;
  if(tokens[p].type != TK_LPARE || tokens[q].type != TK_RPARE) {
    return false;
  }
  for(int i = p + 1; i < q; i++) {
    if(tokens[i].type == TK_LPARE) 
      cnt ++;
    else if(tokens[i].type == TK_RPARE){
      if(--cnt < 0)  return false;
    }
      
  }
  if(cnt != 0) return false;
  return true;
}


uint32_t Find_Oper(int p, int q) {
 // printf("Finding op in the bound %d ~~ %d\n", p, q);
  int cnt = 0, pi = -1, MINN = 10;
  for(int i = q; i >= p; i--) {
    // printf("%c\n", tokens[i].type);
    if(tokens[i].type == TK_RPARE) 
      cnt++;
    else if(tokens[i].type == TK_LPARE) 
      cnt--;
    if(cnt < 0) return -1;
    if(cnt == 0 && MINN > -6 && tokens[i].type == TK_OR) {
      MINN = -6;
      pi = i;
    }
    if(cnt == 0 && MINN > -5 && tokens[i].type == TK_AND) {
      MINN = -5;
      pi = i;
    }
    if(cnt == 0 && MINN > -4 && tokens[i].type == TK_BOR) {
      MINN = -4;
      pi = i;
    }
    if(cnt == 0 && MINN > -3 && tokens[i].type == TK_XOR) {
      MINN = -3;
      pi = i;
    }
    if(cnt == 0 && MINN > -2 && tokens[i].type == TK_BAND) {
      MINN = -2;
      pi = i;
    }
    if(cnt == 0 && MINN > -1 && (tokens[i].type == TK_EQ || tokens[i].type == TK_NEQ)) {
      MINN = -1;
      pi = i;
    }
    if(cnt == 0 && MINN > 0 && (tokens[i].type == TK_LMOV || tokens[i].type == TK_RMOV)) {
      MINN = 0;
      pi = i;
    }
    if(cnt == 0 && MINN > 1 && (tokens[i].type == TK_PLUS || tokens[i].type == TK_MINUS)){
      MINN = 1;
      pi = i;
    }
    if(cnt == 0 && MINN > 2 && (tokens[i].type == TK_MULTI || tokens[i].type == TK_DIVI)){
      MINN = 2;
      pi = i;
    }
    if(cnt == 0 && MINN >= 3 && tokens[i].type == TK_NEG){
      MINN = 3;
      pi = i;
    }
    if(cnt == 0 && MINN > 4 && tokens[i].type == TK_DEREF) {
      MINN = 4;
      pi = i;
    }
  }
  if(pi != -1) return pi;
  Assert(0, "cannot find main operation");
}
 

uint32_t eval(int p, int q, bool* success) {
  // printf("Eval in the %d ~ %d\n", p, q);
  if (p > q) {
    *success = false; 
    printf("Bad P~ Q Range In Fuc Eval\n");
    return 0;
  }
  else if (p == q) {
    // printf("%s\n", tokens[p].str);
    int tmp = atoi(tokens[p].str);
    if(tmp == 0) {
      return isa_reg_str2val(tokens[q].str,success);
    }
    return tmp;
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1, success);
  }
  else {
    int op = Find_Oper(p, q);
    if(op == -1) success = false;
    //printf("OP = %d | tokens[op] type = %d\n", op, tokens[op].type);
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    if(tokens[op].type == TK_NEG){
      val2 = eval(op+1, q, success);
      // printf("In The op '-' val2 == %d\n", val2);
    } else if(tokens[op].type == TK_DEREF) {

    }
    else {
      val1 = eval(p, op - 1, success);
      val2 = eval(op + 1, q, success);
      // printf("In The Normal Op val1 = %u   val2 = %u\n", val1, val2);
    }
    // printf("%d   %d\n", op, tokens[op].type);
    // if(*success == 1) printf("true\n");
    // else printf("Error\n");
    switch (tokens[op].type) {
      case TK_PLUS: return val1 + val2;
      case TK_MINUS: return val1 - val2;
      case TK_NEG: return -val2;
      case TK_DEREF: return isa_reg_str2val(tokens[q].str,success);
      case TK_MULTI: return val1 * val2;
      case TK_DIVI: {
        if(val2 == 0) {
          printf("Error of Div 0\n");
          *success = false;
          return 0;
        }
        // Log("%u %u %u",  val1, val2, val1 / val2);
        return val1 / val2;
      }
      case TK_EQ: return val1 == val2;
      case TK_NEQ: return val1 != val2;
      case TK_AND: return val1 && val2;
      case TK_OR: return val1 || val2;
      case TK_LMOV: return val1 << val2;
      case TK_RMOV: return val1 >> val2;
      default: Assert(0, "Error Main Operation");
    }
  }
}

word_t expr(char *e, bool *success) {
  for(int i = 0; i < 32; i++) {
    tokens[i].type = 0;
    memset(tokens[i].str, 0, sizeof(tokens[i].str));
  }
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  int p = 31;
  while(tokens[p].type == 0) p--;
/* TODO: Insert codes to evaluate the expression. */
  // for(int i = 0; i <= p; i++) {
  //   printf("%s, ", tokens[i].str);
  // }
  // printf("\n");
  return eval(0, p, success);  
  return 0;
}
