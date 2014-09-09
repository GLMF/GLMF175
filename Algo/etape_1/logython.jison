/*
    Définition du langage Logython

    Author: Tristan Colombo <tristan@gnulinuxmag.com>
                            @TristanColombo

    Date: 08-26-2014

    Modification: 08-26-2014
*/


/* Analyse lexicale */
%lex
%%

/* suppression des lignes et espaces superflus */
\n+                                 /* skip */
\s+                                 /* skip */

/* boucle */
"for"                               return 'FOR'

/* test */
"if"                                return 'IF'

/* instruction avancer */
"forward"                           return 'FWD'

/* instruction reculer */
"backward"                          return 'BWD'

/* instruction tourner à gauche */
"turnleft"                          return 'TURNLEFT'

/* instruction tourner à droite */
"turnright"                         return 'TURNRIGHT'

/* range */
"range"                             return 'RANGE'

/* in */
"in"                                return 'IN'

/* identifiant */
[a-zA-Z_]{3}[a-zA-Z0-9_]*\b         return 'IDENTIFIER'

/* entier */
[0-9]+\b                            return 'INTEGER'

/* opérateurs */
"+"                                 return 'ADD'
"-"                                 return 'SUB'
"*"                                 return 'MULT'

/* opérateurs de comparaison */
"=="                                return 'EQUAL'
"!="                                return 'DIFF'
"<="                                return 'LEQ'
">="                                return 'GEQ'
"<"                                 return 'LOWER'
">"                                 return 'GREATER'

/* affectation */
"="                                 return 'ASSIGN'

/* fin de fichier */
<<EOF>>                             return 'EOF'

/* commentaire */
\#.+\n                               return 'COMMENT'

/* ouverture blocs mode Python */
":"                                 return 'COLON'

/* blocs */
"{"                                 return 'BLOCKSTART'
"}"                                 return 'BLOCKEND'

/* parenthèses */
"("                                 return 'LEFTBRACKET'
")"                                 return 'RIGHTBRACKET'

/* tout le reste représente une erreur */
.                                   return 'INVALID'

/lex


/* Analyse syntaxique */
%start program
%%

program
    : instructions EOF
        {
            console.log($1);
            return $1; 
        }
    ;

instructions
    : instructions instr
        {
            $$ = $1 + $2;
        }
    | instr
        {
            $$ = $1;
        }
    ;

instr
    : declare_variable
        {
            $$ = $1;
        }
    | comment
        {
        }
    | 'FWD' 'IDENTIFIER'
        {
            $$ = "Avancer de " + $2 + " case(s)\n";
        }
    | 'FWD' 'INTEGER'
        {
            $$ = "Avancer de " + $2 + " case(s)\n";
        }
    | 'BWD' 'IDENTIFIER'
        {
            $$ = "Reculer de " + $2 + " case(s)\n";
        }
    | 'BWD' 'INTEGER'
        {
            $$ = "Reculer de " + $2 + " case(s)\n";
        }
    | 'TURNLEFT'
        {
            $$ = "Tourner à gauche\n";
        }
    | 'TURNRIGHT'
        {
            $$ = "Tourner à droite\n";
        }
    | if_condition
        {
            $$ = $1;
        }
    | loop
        {
            $$ = $1;
        }
    ;

declare_variable
    : 'IDENTIFIER' 'ASSIGN' value
        {
            $$ = "var " + $1 + " = " + $3 + "\n";
        }
    ;

value
    : 'INTEGER'
        {
            $$ = $1;
        }
    | 'IDENTIFIER'
        {
            $$ = $1;
        }
    |   'INTEGER' operator value
        {
            $$ = $1 + $2 + $3;
        }
    |   'IDENTIFIER' operator value
        {
            $$ = $1 + $2 + $3;
        }

    ;

operator
    : 'ADD'
        {
            $$ = " + ";
        }
    | 'SUB'
        {
            $$ = " - ";
        }
    | 'MULT'
        {
            $$ = " * ";
        }
    ;

test
    : 'EQUAL'
        {
            $$ = " == ";
        }
    | 'DIFF'
        {
            $$ = " != ";
        }
    | 'LEQ'
        {
            $$ = " <= ";
        }
    | 'GEQ'
        {
            $$ = " >= ";
        }
    | 'LOWER'
        {
            $$ = " < ";
        }
    | 'GREATER'
        {
            $$ = " > ";
        }
    ;

comment
    : 'COMMENT'
        {
            $$ = '';
        }
    ;

if_condition
    : 'IF' 'IDENTIFIER' test value 'COLON' block
        {
            $$ = "\nSi ( " + $2 + $3 + $4 + " ) alors\n" + $6;
        }
    ;

block
    : 'BLOCKSTART' instructions 'BLOCKEND'
        {
            $$ = "/* Début Bloc */\n" + $2 + "/* Fin bloc */\n\n";
        }
    ;

loop
    : 'FOR' 'IDENTIFIER' 'IN' range_fct 'COLON' block
        {
            $$ = "\nPour " + $2 + " variant " + $4 + " Faire\n" + $6;
        }
    ;

range_fct
    : 'RANGE' 'LEFTBRACKET' 'INTEGER' 'RIGHTBRACKET'
        {
            $$ = "de 0 à " + $3 + " - 1";
        }
    | 'RANGE' 'LEFTBRACKET' 'IDENTIFIER' 'RIGHTBRACKET'
        {
            $$ = "de 0 à " + $3 + " - 1";
        }
    ;
