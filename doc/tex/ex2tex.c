/**
 ** ex2tex.c ---- append examples to info docs
 **
 ** Copyright (c) 2001 Vicente Fuentes Gea
 ** [e-mail: vfg@servidor.unam.mx].
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>

char *separa = "@c -----------------------------------------------------------------------------";
FILE *fp;
char **ap;
int nhd;

struct seccion
{
 char *Node, *Prev, *Up;
} s;

void insertaCadena(char *linea, int x, char *cadena)
{
 int ll = strlen(linea), lc = strlen(cadena);

 if( x > ll )
 {
        memset(linea + ll, ' ', x);
        memcpy(linea + x - 1, cadena, lc);
        linea[lc + x - 1] = '\0';
 }
 else
 {
        char *tmp = calloc(strlen(linea + x) + 1, 1);
        strcpy(tmp, linea + x - 1);
        strcpy(linea + x - 1, cadena);
        strcpy(linea + x - 1 + lc, tmp);
        free(tmp);
 }

}

void InsertaCaracter(char *linea, char carAinsertar, char carAubicar)
{
   char *aqui = strchr(linea, carAubicar);

   while (aqui)
   {
                char tmp[180];
                strcpy(tmp, aqui);
                *aqui = carAinsertar;
                strcpy(aqui + 1, tmp);
      aqui = strchr(aqui + 2, carAubicar);
   }
}

void DescargaArchivo(FILE *p, char *na)
{
  int i;
  char linea[180];
  FILE *ae = fopen(na, "r");

  fputs("@example\n", p);

  do
  {
          fgets(linea, 179, ae);
        
          if(feof(ae)) break;
                              /* Procesamiento de la l¡nea le¡da */

          InsertaCaracter(linea, '@', '@');    /*  @@  */
          InsertaCaracter(linea, '@', '{');    /*  @{  */
          InsertaCaracter(linea, '@', '}');    /*  @}  */

     for(i = 0; i < nhd; i++) /* Colocar las referencias a los include */
     {
             char *arch =strstr(linea, ap[i]);

        if(strcmp(na, ap[i]) == 0) continue; /* no tocar el propio include */

             if( arch )       /* ¨est  referido el archivo include? */
        {
           char *coment = strchr(arch, '/');
                char cadena[40];

           if(coment)     /* si hay cometario, incr£stalo dentro de ‚l */
           {
             coment = strchr(coment, '*');
             if(coment)
             {
                           strcpy(cadena, "@pxref{");
                                strcat(cadena, ap[i]);
                                strcat(cadena, "} ");
                                              insertaCadena(coment, 3, cadena);
                           break;
             }
           }                  /* No hay comentario, hay que colocarlo */
           strcpy(cadena, "/* @pxref{");
                strcat(cadena, ap[i]);
                strcat(cadena, "} */\n");
                linea[strlen(linea) - 1] = ' ';  /* quitamos el \n */
                              insertaCadena(linea, strlen(linea) + 2, cadena);
           break;
        }
     }
          fputs(linea, p);
  }
  while(1);

  fputs("@end example\n", p);
  fclose(ae);
}

void FinSeccion(char *Up, char *Next)
{
  fprintf(fp,"@node  %s, %s, %s, %s\n", s.Node, Next, s.Prev, Up);
  fprintf(fp, "%s  %s\n", "@unnumberedsec ", s.Node);
  DescargaArchivo(fp, s.Node);
  fprintf(fp, "%s\n", separa);
}

void DescargaArchivos(glob_t *r)
{
  int i;

  fprintf(fp, "@end menu\n");
  fprintf(fp, "%s\n", separa);

  if(r->gl_pathc == 1)          /*  ¨Un solo archivo en el directorio?  */
  {
      s.Node = r->gl_pathv[0];
      if(s.Prev != NULL) free(s.Prev);
      s.Prev= strdup(s.Up);
      return;
  }

  fprintf(fp, "@node  %s, %s, %s, %s\n", r->gl_pathv[0], r->gl_pathv[1],
                                                            s.Up, s.Up);
  fprintf(fp, "%s  %s\n", "@unnumberedsec ", r->gl_pathv[0]);
  DescargaArchivo(fp, r->gl_pathv[0]);
  fprintf(fp,"%s\n",separa);

  for(i = 1; i < r->gl_pathc - 1 ; i++)
  {
     fprintf(fp,"@node  %s, %s, %s, %s\n", r->gl_pathv[i], r->gl_pathv[i + 1],
                                                    r->gl_pathv[i - 1], s.Up);
     fprintf(fp, "%s  %s\n", "@unnumberedsec ", r->gl_pathv[i]);
     DescargaArchivo(fp, r->gl_pathv[i]);
     fprintf(fp,"%s\n",separa);
  }

  s.Node = r->gl_pathv[i];
  s.Prev = r->gl_pathv[i - 1];
}

void IniSeccion(FILE *p, char *Node, char *Next, char *Prev, char *Up)
{
  fprintf(p,"@node %s, %s, %s, %s\n", Node, Next, Prev, Up);
  fprintf(p,"@unnumberedsec %s\n@menu\n", Node);
  fp = p;
  if(s.Up != NULL) free(s.Up);
  s.Up = strdup(Node);
}

void FormaIndice(glob_t *r)
{
  int i;

  for(i = 0; i < r->gl_pathc; i++)
      fprintf(fp, "* %s::\n", strlwr(r->gl_pathv[i]));
}

int main()
{
  glob_t r, rh;
  FILE *p0, *p;

  p0 = fopen("grx2_0.tex", "r");
  p = fopen("grx2_2.tex", "w");

  do         /* Copiamos grx2_0.tex a grx2_2.tex  */
  {
                char linea[180];
                fgets(linea, 179, p0);
                fputs(linea, p);
  } while( !feof(p0) );
  fclose(p0);

  chdir("..");
  chdir("..");
  chdir("test");   /* Leemos los nombres de los archivos que nos interesan */
  glob("*.c", 0 , 0, &r);
  nhd = r.gl_pathc; /* # de archivos *.c (variable global) */
  glob("*.h", GLOB_APPEND , 0, &r);
  glob("*.dat", GLOB_APPEND, 0, &r);

  if(r.gl_pathc == 0)
  {
          chdir("..");
          chdir("doc/tex");
          fclose(p);
     puts("Directorio sin los archivos requeridos. Programa abortado.");
     exit(1);
  }

  ap = &r.gl_pathv[nhd];    /* inicio de los *.h y *.dat (variable global) */
  nhd = r.gl_pathc - nhd;   /* # de archivos *.h y *.dat en \test */

  /*                Node               Next                  Prev           Up */
  IniSeccion(p, "Test examples", strlwr(r.gl_pathv[0]), "BGI interface",
                                                      "A User Manual For GRX2");
  FormaIndice(&r);        /* Utilizando los nombre de los archivos  */
  DescargaArchivos(&r);
  FinSeccion("Test examples", "Includes");/* Fin de Text examples sigue Includes */

  chdir("..");            /* Ahora leemos los includes de GRX20 */
  chdir("include");
  glob("grx*.h", 0, 0, &rh);

  if(rh.gl_pathc == 0)
  {
          chdir("..");
          chdir("doc/tex");
          fclose(p);
     globfree(&r);
     puts("Directorio sin los archivos requeridos. Programa abortado.");
     exit(1);
  }

  nhd = 0;
  /*               Node               Next                  Prev           Up */
  IniSeccion(p, "Includes", strlwr(rh.gl_pathv[0]), r.gl_pathv[r.gl_pathc - 1],
                                                    "A User Manual For GRX2");
  FormaIndice(&rh);
  DescargaArchivos(&rh);
  FinSeccion("Includes", ""); /* Fin de Includes y adi¢s */

  chdir("..");
  chdir("doc/tex");
  fclose(p);

  globfree(&r);
  globfree(&rh);

  return 0;
}

