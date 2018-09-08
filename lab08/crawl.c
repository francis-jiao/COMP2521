// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "queue.h"
#include "set.h"
#include "graph.h"
#include "html.h"
#include "url_file.h"

#define BUFSIZE 1024

void setFirstURL(char *, char *);
void normalise(char *, char *, char *, char *, int);

int main(int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int  maxURLs;

	if (argc > 2) {
		strcpy(baseURL,argv[1]);
		setFirstURL(baseURL,firstURL);
		maxURLs = atoi(argv[2]);
		if (maxURLs < 40) maxURLs = 40;
	}
	else {
		fprintf(stderr, "Usage: %s BaseURL MaxURLs\n",argv[0]);
		exit(1);
	}
    
	Queue todoQueue = newQueue();
	enterQueue(todoQueue, firstURL);
	Graph URLgraph = newGraph(maxURLs);
	Set seenURL = newSet();
	
	while (emptyQueue(todoQueue) == 0 && nVertices(URLgraph) <= maxURLs) {
	    char *nextURL = leaveQueue(todoQueue);
	    if (!(handle = url_fopen(nextURL, "r"))) {
	        fprintf(stderr, "couldn't open fread output file\n");
	        return 1;
	    }
	    while (!url_feof(handle)) {
	        url_fgets(buffer, sizeof(buffer), handle);
	        int pos = 0;
	        char result[BUFSIZE];
	        memset(result, 0, BUFSIZE);
	        while ((pos = GetNextURL(buffer, nextURL, result, pos)) > 0) {
	            if (nVertices(URLgraph) <= maxURLs || isConnected(URLgraph, nextURL, result) == 0) {
	                //sleep(0.1);
	                addEdge(URLgraph, nextURL, result);
	            }
	            if (isElem(seenURL,result) == 0) {
	                insertInto(seenURL, result);
	                enterQueue(todoQueue, result);
	            }
	            memset(result, 0, BUFSIZE);
	        }
	    }
	    url_fclose(handle);
	    sleep(1);
	}

	if (!(handle = url_fopen(firstURL, "r"))) {
		fprintf(stderr,"Couldn't open %s\n", next);
		exit(1);
	}
	while(!url_feof(handle)) {
		url_fgets(buffer,sizeof(buffer),handle);
		//fputs(buffer,stdout);
		int pos = 0;
		char result[BUFSIZE];
		memset(result,0,BUFSIZE);
		while ((pos = GetNextURL(buffer, firstURL, result, pos)) > 0) {
			printf("Found: '%s'\n",result);
			memset(result,0,BUFSIZE);
		}
	}
	url_fclose(handle);
	sleep(1);
	
	showGraph(URLgraph, 0);
	showGraph(URLgraph, 1);
	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
void setFirstURL(char *base, char *first)
{
	char *c;
	if ((c = strstr(base, "/index.html")) != NULL) {
		strcpy(first,base);
		*c = '\0';
	}
	else if (base[strlen(base)-1] == '/') {
		strcpy(first,base);
		strcat(first,"index.html");
		base[strlen(base)-1] = '\0';
	}
	else {
		strcpy(first,base);
		strcat(first,"/index.html");
	}
}
