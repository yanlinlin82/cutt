#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SAMPLE_LINES 10
#define MAX_COLUMN 999

static char *buf = NULL;
static int buf_size = 0;
static int length = 0;

static int sep[MAX_COLUMN] = { };
static int count = 0;

static void append(char c)
{
	if (length == buf_size) {
		char *p = malloc(buf_size + 1024);
		if (buf && length) {
			memcpy(p, buf, length);
		}
		free(buf);
		buf = p;
		buf_size += 1024;
	}
	buf[length++] = c;
}

static void output_with_sep(FILE *fp)
{
	int c, k, i, j, spaces, pos;

	for (k = 0; k < length || !feof(fp);) {
		i = 0;
		spaces = 0;
		pos = 0;
		for (;;) {
			if (k < length) {
				c = buf[k++];
			} else {
				c = fgetc(fp);
			}
			if (c == EOF) break;
			if (isgraph(c)) {
				++pos;
				if (i < count && pos >= sep[i]) {
					printf("\t");
					++i;
				} else {
					for (j = 0; j < spaces; ++j) {
						printf(" ");
					}
				}
				spaces = 0;
				printf("%c", c);
			} else {
				pos += (c == '\t' ? 8 : (c == ' '));
				spaces += (c == '\t' ? 8 : (c == ' '));
			}
			if (c == '\n') {
				printf("\n");
				break;
			}
		}
	}
}

int main(int argc, char * const argv[])
{
	FILE *fp;
	int line, c, pos, flag, confirm;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 1;
	}

	fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "Error: Can not open file '%s'!\n", argv[1]);
		return 1;
	}
	for (line = 0; line < SAMPLE_LINES; ++line) {
		pos = 0;
		flag = 0;
		confirm = 0;
		while ((c = fgetc(fp)) != '\n') {
			if (c == EOF) goto out;
			append((char)c);
			if (isgraph(c)) {
				++pos;
				if (flag) {
					flag = 0;
					if (line == 0) {
						if (count < MAX_COLUMN) {
							sep[count++] = pos;
						} else {
							fprintf(stderr, "Error: Too many spaces!\n");
							fclose(fp);
							return 1;
						}
					} else if (confirm < count) {
						int i, j;
						for (i = confirm; i < count; ++i) {
							if (sep[i] >= pos) {
								break;
							}
						}
						if (i > confirm) {
							for (j = 0; i + j < count; ++j) {
								sep[confirm + j] = sep[i + j];
							}
							count = confirm + j;
						}
						if (sep[confirm] == pos) {
							++confirm;
						}
					}
				}
			} else {
				pos += (c == '\t' ? 8 : (c == ' '));
				flag = 1;
			}
		}
		if (c == '\n') append('\n');
	}
out:
	output_with_sep(fp);
	fclose(fp);
	return 0;
}
