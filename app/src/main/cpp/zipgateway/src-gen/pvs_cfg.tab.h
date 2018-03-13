/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_VAR_LIB_JENKINS_JOBS_FULL_S2_GATEWAY_RELEASE_WORKSPACE_SRC_GEN_PVS_CFG_TAB_H_INCLUDED
# define YY_YY_VAR_LIB_JENKINS_JOBS_FULL_S2_GATEWAY_RELEASE_WORKSPACE_SRC_GEN_PVS_CFG_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUM = 258,
    QRBLOCK = 259,
    HEADER = 260,
    DEVICE = 261,
    QR = 262,
    TYPE = 263,
    DSK = 264,
    S2 = 265,
    SS = 266,
    VALUE = 268,
    TLVS = 269,
    ASCIIHEX = 270,
    DEVICEPTR = 271,
    TLVTYPE = 272,
    STR = 273,
    TLV = 274
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{

  /* NUM  */
  int NUM;
  /* HEADER  */
  int HEADER;
  /* DEVICE  */
  int DEVICE;
  /* QR  */
  int QR;
  /* TYPE  */
  int TYPE;
  /* S2  */
  int S2;
  /* SS  */
  int SS;
  /* DSK-BOOTMODE  */
  int yytype_12;
  /* VALUE  */
  int VALUE;
  /* TLVS  */
  int TLVS;
  /* TLVTYPE  */
  int TLVTYPE;
  /* dsk-bootmode-spec  */
  int yytype_33;
  /* DEVICEPTR  */
  struct provision* DEVICEPTR;
  /* pvscfg  */
  struct provision* pvscfg;
  /* devspec  */
  struct provision* devspec;
  /* DSK  */
  struct pvs_parse_buf* DSK;
  /* ASCIIHEX  */
  struct pvs_parse_buf* ASCIIHEX;
  /* STR  */
  struct pvs_parse_buf* STR;
  /* dsk-spec  */
  struct pvs_parse_buf* yytype_32;
  /* tlv-value-spec  */
  struct pvs_parse_buf* yytype_37;
  /* TLV  */
  struct pvs_tlv* TLV;
  /* tlv-list-spec  */
  struct pvs_tlv* yytype_34;
  /* tlv-list  */
  struct pvs_tlv* yytype_35;
  /* tlvspec  */
  struct pvs_tlv* tlvspec;
  /* QRBLOCK  */
  uint16_t QRBLOCK;
#line 124 "/var/lib/jenkins/jobs/Full_S2_Gateway_release/workspace/src-gen/pvs_cfg.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_VAR_LIB_JENKINS_JOBS_FULL_S2_GATEWAY_RELEASE_WORKSPACE_SRC_GEN_PVS_CFG_TAB_H_INCLUDED  */
