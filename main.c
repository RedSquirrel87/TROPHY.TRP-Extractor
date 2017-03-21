/*
   main.c

   This file is part of trpex project.
   Copyright (C) 2010 Red Squirrel.

   trpex is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3 of the License, or (at your
   option) any later version.

   trpex is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with trpex. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
/* For MkDir: */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
/* For ntohl */
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    //Credits:
    printf("\t----------------------------------------\n\t| TROPHY.TRP Extractor by Red Squirrel |\n\t| http://www.psp-cheats.it/redsquirrel |\n\t----------------------------------------\n");

    //Check params
    if ((argc < 2) || (argc > 3))
    {
        printf("ERROR: wrong number of params!\nUsage:\n\ttrpex \"/path/of/your/TROPHY.TRP\" \"/output/folder\"\n");
        return 0;
    }

    // Var. Declarations
    FILE *f, *o;
    int c = 64, i = 0, z, fine;
    char s[16], *h, path[128];

    //Let's start!
    printf("\nCreating output folder...");
    if (mkdir(argv[2], 0777) < 0) printf("FAILED!");
    else printf("OK!");

    printf("\nOpening input file...");
    f = fopen(argv[1],"r");
    if (f == NULL)
    {
        printf("FAILED!");
        printf("\n\nERROR: unable to open input file!\n");
        return 0;
    }
    else printf("OK!");

    printf("\nGetting header size...");
    if (fseek(f,100,SEEK_SET) < 0)
    {
        printf("SEEK FAILED!");
        printf("\n\nERROR: unable to get header size!\n");
        fclose(f);
        return 0;
    }
    if (fread(&fine,4,1,f) < 1)
    {
        printf("READ FAILED!");
        printf("\n\nERROR: unable to get header size!\n");
        fclose(f);
        return 0;
    }
    fine = ntohl(fine);
    printf("OK!");

    while (c < fine)
    {
        printf("\n\nGetting filename...");
        if (fseek(f,c,SEEK_SET) < 0)
        {
            printf("SEEK FAILED [1]!");
            printf("\n\nERROR: unable to get internal filenames!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        if (fread(&s,16,1,f) < 1)
        {
            printf("READ FAILED [1]!");
            printf("\n\nERROR: unable to get internal filenames!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        printf("%s",s);

        printf("\n>>\tGetting file content...");
        if (fseek(f,20,SEEK_CUR) < 0)
        {
            printf("SEEK FAILED [2]!");
            printf("\n\nERROR: unable to get internal file content!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        if (fread(&i,4,1,f) < 1)
        {
            printf("READ FAILED [2]!");
            printf("\n\nERROR: unable to get internal file content!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        i = ntohl(i);

        if (fseek(f,4,SEEK_CUR) < 0)
        {
            printf("SEEK FAILED [3]!");
            printf("\n\nERROR: unable to get internal file content!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        if (fread(&z,4,1,f) < 1)
        {
            printf("READ FAILED [3]!");
            printf("\n\nERROR: unable to get internal file content!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        z = ntohl(z);
        if (fseek(f,i,SEEK_SET) < 0)
        {
            printf("SEEK FAILED [4]!");
            printf("\n\nERROR: unable to get internal file content!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        h = (char*)malloc(z);
        if (fread(h,z,1,f) < 1)
        {
            printf("READ FAILED [4]!");
            printf("\n\nERROR: unable to get internal file content!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        printf("OK!");

        printf("\n>>\tOpening output file...");
        if (argv[2] != NULL) sprintf(path,"%s/%s",argv[2],s);
        else sprintf(path, "%s", s);
        o = fopen(path,"w");
        if (o == NULL)
        {
            printf("FAILED!");
            printf("\n\nERROR: unable to open output file!\n");
            c = fine;
            fclose(f);
            return 0;
        }
        else
        {
            printf("OK!");
            printf("\n>>\tWriting output file content...");
            if (fwrite(h,z,1,o) < 1)
            {
                printf("FAILED!");
                printf("\n\nERROR: unable to write output file content!\n");
                fclose(f);
                fclose(o);
                c = fine;
                return 0;
            }
            else printf("DONE!");
        }
        fclose(o);
        c += 64;
        free(h);
    }

    fclose(f);
    printf("\n\nAll done! Bye bye :-)\n\n");
    return 0;
}
