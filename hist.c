/* Make  histogram of a list of data				*
*	option -n specifies number of bins			*
*	option -s specifies size of bin				*
*	option -x specifies lower and upper limits of histogram	*
*	option -g specifies output in form suitable for "graph"	*
*			or "axis"				*
*		  if -g 1 we get a full bar for each bin	*
*		  if -g 2 the bars don't go down to the origin	*
*	option -m specifies the line type for use with "axis"   *
*	option -cm specifies the color for use with "axis"   	*
*								*/

#include <stdio.h>
#include <math.h>
int mkstemp(char *);

main(int argc,char **argv) {
int *hist,ndata,nbins,i,first,gflag=0;
int line_modeflag=0,line_mode;
int color_modeflag=0,color_mode;
double binsize,x,xmin,xmax;
char tempfile[19]="/usr/tmp/hstXXXXXX";
FILE *tempfp,*fopen();
int fd;

	/* read data, count it, and find min and max */
	fd = mkstemp(tempfile);
	tempfp = fopen(tempfile,"w");
	if(tempfp == NULL){fprintf(stderr,"Can't open temp file\n"); exit(-1);}
	xmin = xmax = binsize = 0.0; nbins = 0;
	first = 1;	ndata = 0;
	while(scanf("%le",&x) == 1){
		if(first){ xmin = x; xmax = x; }
		else{
			if( x < xmin ) xmin = x;
			if( x > xmax ) xmax = x;
		}
		first = 0;
		fprintf(tempfp,"%le\n",x);
		ndata++;
	}
	xmax += (xmax-xmin)/1000000.0;
	xmin -= (xmax-xmin)/1000000.0;
	/* now open file for reading */
	fclose(tempfp);
	tempfp = fopen(tempfile,"r");

	/* read arguments and compute parameters */
	for(i=1;i<argc;i++){
		if(strcmp(argv[i],"-n")==0){
			if(sscanf(argv[++i],"%d",&nbins) != 1)
				fprintf(stderr,"bad argument %s\n",argv[i]);
		}
		else if(strcmp(argv[i],"-s")==0){
			if(sscanf(argv[++i],"%le",&binsize) != 1)
				fprintf(stderr,"bad argument %s\n",argv[i]);
		}
		else if(strcmp(argv[i],"-g")==0){
		   	if(++i == argc) gflag =1;
			else if(sscanf(argv[i],"%d",&gflag) != 1)  {
			   --i;
			   gflag = 1;	/* make output suitable for axis */
			}
			if(gflag > 2 || gflag < 0)  {
			 fprintf(stderr,"unimplemented output flag %d\n",gflag);
			 gflag=1;
			}
		}
		else if(strcmp(argv[i],"-m")==0){
			line_modeflag=1;
			if(sscanf(argv[++i],"%d",&line_mode) != 1)
			   fprintf(stderr,"bad argument after -m %s\n",argv[i]);
		}
		else if(strcmp(argv[i],"-cm")==0){
			color_modeflag=1;
			if(sscanf(argv[++i],"%d",&color_mode) != 1)
			   fprintf(stderr,"bad argument after -cm %s\n",argv[i]);
		}
		/* replace xmin and xmax if desired */
		else if(strcmp(argv[i],"-x")==0){
			if(sscanf(argv[++i],"%le",&xmin) != 1)
				fprintf(stderr,"bad argument %s\n",argv[i]);
			if(sscanf(argv[++i],"%le",&xmax) != 1)
				fprintf(stderr,"bad argument %s\n",argv[i]);
		}
		else fprintf(stderr,"bad argument %s\n",argv[i]);
	}
	if( binsize == 0.0 && nbins == 0){	/* use default */
		nbins = 10;	binsize = (xmax - xmin)/nbins;
	}
	else if (binsize == 0.0 && nbins != 0)
		binsize = (xmax - xmin)/nbins;
	else if (binsize != 0.0 && nbins == 0){
		nbins = ceil((xmax - xmin)/binsize);
		xmax = xmin + nbins * binsize;
	}
	else {
		fprintf(stderr,"Warning: conflicting options -n and -s\n");
		xmax = xmin + nbins * binsize;
	}

	/* Now make the histogram */
	hist = (int *)malloc(nbins*sizeof(int));
	for(i=0;i<nbins;i++)hist[i]=0;
	while(fscanf(tempfp,"%le",&x) == 1){
		i = (x-xmin)/binsize;
		if(i >= 0 && i < nbins)hist[i]++;
	}

	/* Output lines */
	if(line_modeflag == 1 && gflag != 0) printf("\n# m %d\n",line_mode);
	if(color_modeflag == 1 && gflag != 0) printf("\n# cm %d\n",color_mode);
	if(gflag == 2) printf("%le %d\n",xmin,0);
	for(x=xmin,i=0;i<nbins;i++,x += binsize){
		if(gflag == 1)printf("%le %d\n%le %d\n%le %d\n%le %d\n",
		x,0,x,hist[i],x+binsize,hist[i],x+binsize,0);
		else if (gflag == 2)
		    printf("%le %d\n%le %d\n",x,hist[i],x+binsize,hist[i]);
		else printf("%le %le %d\n",x,x+binsize,hist[i]);
	}
	if(gflag == 2) printf("%le %d\n",x,0);
	fclose(tempfp);
	unlink(tempfile);
}
