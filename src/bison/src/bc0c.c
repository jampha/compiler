#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "sym_tab.h"
#include "gen.h"

int parse();

struct gen_info *gi[] = {
	&gen_pl0sim,
	&gen_c,
	&gen_spim,
	NULL,
};

/* c0.y */
extern struct sym_tab *symtab;

/* c0.lex */
extern FILE *yyin;

int main(int argc, char **argv)
{
	/* get options */
	FILE *infile;
	int i;
	char *mach = "pl0sim";
	char *inf = NULL;
	char *outf = "a.out";
	int opt;
	while ((opt = getopt(argc, argv, "m:o:h")) != -1) {
		switch (opt) {
		case 'o':
			outf = optarg;
			break;
		case 'm':
			mach = optarg;
			break;
		case 'h':
		default: /* '?' */
			fprintf(stderr,
				"%s: Bison C0 compiler\n"
				"Usage: %s [-m mach] [-o obj_file] src_file\n\n"
				"o  output in obj_file (default: a.out)\n"
				"m  select a object machine (default:pl0sim)\n",
				argv[0],
				argv[0]);
			fprintf(stderr,
				"\nAvailable object machine:\n\n");
			for(i = 0; gi[i]; i++)
			fprintf(stderr,
				"%-10s %s\n",
				gi[i]->name,
				gi[i]->info);
			exit(1);
		}
	}
	
	if (optind >= argc) {
		fprintf(stderr,
			"%s: Expected argument after options. use -h.\n",
			argv[0]);
		exit(1);
	}

	if (optind != argc - 1) {
		fprintf(stderr,
			"%s: Too many argument.\n",
			argv[0]);
		exit(1);
	}
	
	inf = argv[optind];

	if(strcmp(inf, "-") == 0)
		infile = stdin;
	else
		infile = fopen(inf, "r");
	if(!infile) {
		fprintf(stderr,
			"%s: File %s can't be opened.\n",
			argv[0],
			inf);
		exit(1);
	}
	
	symtab = symtab_new(NULL);
	yyin = infile;
	if(parse())
		return 1;
	for(i = 0; gi[i]; i++)
	{
		if(strcmp(mach, gi[i]->name) == 0)
		{
			fprintf(stderr, "use machine %s.\n", gi[i]->name);
			gi[i]->gen_code(symtab, outf);
		}
	}
	
	fclose(infile);
	return 0;
}
