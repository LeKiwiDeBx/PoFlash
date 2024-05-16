/*
 * main.h
 */

#ifndef _MAIN_H_
#define _MAIN_H_

typedef struct s_xgettext_args
{

      /*
      Synopsis :
           xgettext [OPTION] [INPUTFILE]
      */
      /* Input file location: */
      char *inputfile; // input files string list
                       /*  gchar *files_from; // -f get list of input files from FILE
                        gchar *directory;  // -D add DIRECTORY to list for input files search
                */
      /* Output file location: */
      /*  char *default_domain; // -d use NAME.po for output (instead of messages.po) |
      */
       char *output;         // -o write output to specified file .po or .pot      | -d OR -o
       
       char *output_dir;     // -p output files will be placed in directory DIR

      /* Choice of input file language: */
      /*
      char *language;  // -L --language=name recognise the specified language (C, C++, ObjectiveC, PO,
      */
      char *cplusplus; // -C --c++ shorthand for --language=C++ 

      /* Input file interpretation: */
      char *from_code; // --from-code=NAME encoding of input files (except for Python, Tcl, Glade) by default ASCII

      /* Operation mode: */
      /*
      char *join_existing; // -j --join-existing
      char *exclude_file;  // -x --exclude-file=FILE.po entries from FILE.po are not extracted
*/
      /* Language specific options: */
      char *add_comments; // -c --add-comments=TAG place comment block with TAG (or those preceding keyword lines) in output file
      /*
            char *extract_all;  // -a --extract-all extract all strings (only languages C, C++, ObjectiveC, ..
      */
      char *keyword; // -k --keyword=KEYWORD additional keyword to be looked for (without WORD means not to use default keywords)

      /*
      char *flag;         // --flag=WORD:ARG:FLAG additional flag for strings inside the argument number ARG of keyword WORD
      char *trigraphs;    // -T --trigraphs understand ANSI C trigraphs for input (only languages C, C++, ObjectiveC)
      char *qt;           // --qt recognize Qt format strings (only language C++)
      char *kde;          // --kde recognize KDE format strings (only language C++)
      char *boost;        // --boost recognize Boost format strings (only language C++)
      char *debug;        // --debug  more detailed formatstring recognition result
       */

      /* Output details: */
      /*
      char *no_escape;          // -e --no-escape do not use C escapes in output (default)
      char *escape;             // -E --escape use C escapes in output, no extended chars
      */
      char *force_po; // --force-po write PO file even if empty
      /*
      char *indent;             // -i --indent write the .po file using indented style
      char *no_location;        // --no-location do not write '#: filename:line' lines
      char *add_location;       // -n --add-location generate '#: filename:line' lines (default)
      char *strict;             // --strict write out strict Uniforum conforming .po file
      char *properties_output;  // --properties-output write out a Java .properties file
      char *width;              // -w --width=NUMBER set output page width
      char *no_wrap;            // --no-wrap do not break long message lines, longer than the output page width, into several lines
      char *sort_output;        // -s --sort-output generate sorted output
      char *sort_by_file;       // -F --sort-by-file sort output by file location
      char *omit_header;        // --omit-header don't write header with 'msgid ""' entry
      */
      char *copyright_holder; // --copyright-holder=STRING set copyright holder in output
      /*
      char *foreign_user;       // --foreign-user omit FSF copyright in output for foreign user
      */
      char *package_name;       // --package-name=PACKAGE set package name in output
      char *package_version;    // --package-version=VERSION set package version in output
      char *msgid_bugs_address; // --msgid-bugs-address=EMAIL@ADDRESS set report address for msgid bugs
      /*
      char *msgstr_prefix;      // -m --msgstr-prefix=STRING use STRING or "" as prefix for msgstr entries
      char *msgstr_suffix;      // -M --msgstr-suffix=STRING use STRING or "" as suffix for msgstr entries
      */
} *xgettext_args;

#endif