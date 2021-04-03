#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CYELLOW "\x1b[38;2;255;255;0m"
#define CBLUE "\x1b[38;2;0;0;255m"
#define CRED "\x1b[38;2;255;0;0m"
#define CWHITE "\x1b[38;2;255;255;255m"

#define OS_ARTIX 7599908386896181248
#define OS_ARCH	7163059223041243136

#define PCOL CYELLOW
#define SCOL CWHITE
#define GCOL CRED

const char* artix_linux =
GCOL
"    /\\    \n"
"   /\\_\\   \n"
"  /\\_ /\\  \n"
" / _ / /\\ \n"
"/_/    \\_\\\n";

const char* arch_linux =
GCOL
"    /\\    \n"
"   /\\ \\   \n"
"  /    \\  \n"
" /  /\\ -\\ \n"
"/_-'  '-_\\\n";

struct mem_info
{
	int mem_total;
	int mem_used;
	int mem_free;
};

long get_os(char** pbuf)
{
	char* buf = *pbuf;
	FILE *fp;
	long oskey = 0;
	int len = 0;

	fp = fopen("/etc/os-release", "r");
	fscanf(fp, "%s", buf);

	buf += 6;
	len = strlen(buf);
	while (buf[len-1] != '"')
	{
		fscanf(fp, "%s", buf + strlen(buf));
		len = strlen(buf);
	}

	buf[--len] = 0;

	//buf = "ArtixLinux";
	//len = 10;

	//printf("%s\n", buf);

	for (int i = 0; i < len; i++)
	{
		oskey ^= buf[i];
		oskey <<= 8;
	}

	//printf("key: %ld\n", oskey);

	fclose(fp);
	*pbuf = buf;
	return oskey;
}

struct mem_info get_mem_info(void)
{
	struct mem_info info;
	FILE *fp;
	char buff[255];

	fp = fopen("/proc/meminfo", "r");
	fscanf(fp, "%s", buff);
	fscanf(fp, "%d", &info.mem_total);
	fscanf(fp, "%s", buff);
	fscanf(fp, "%s", buff);
	fscanf(fp, "%s", buff);
	fscanf(fp, "%s", buff);
	fscanf(fp, "%s", buff);
	fscanf(fp, "%d", &info.mem_free);
	info.mem_used = info.mem_total - info.mem_free;
	fclose(fp);

	return info;
}

int get_pkg_count(void)
{
	int pkg_count;
	FILE *fp;
	fp = popen("pacman -Qq | wc -l", "r");
	if (fp == NULL) exit(1);
	fscanf(fp, "%d", &pkg_count);
	pclose(fp);
	return pkg_count;
}

void get_kernel_name(char buf[1024])
{
	FILE *fp;
	fp = popen("uname -r", "r");
	if (fp == NULL) exit(1);
	fscanf(fp, "%s", buf);
	pclose(fp);
}
void get_names(char user[1024], char host[1024])
{
	FILE *fp;
	fp = popen("echo $USER", "r");
	if (fp == NULL) exit(1);
	fscanf(fp, "%s", user);
	fp = popen("echo $HOSTNAME", "r");
	if (fp == NULL) exit(1);
	fscanf(fp, "%s", host);
	pclose(fp);
}

void print_logo(long os)
{
	switch (os)
	{
		case OS_ARTIX:
			printf(artix_linux);
			break;
		case OS_ARCH:
			printf(arch_linux);
			break;
	}
}

int main(void)
{
	char os_name_buf[1024] = {0};
	char* os_name = os_name_buf;
	char kernel_name[1024] = {0};
	char username[1024] = {0};
	char hostname[1024] = {0};
	long os = get_os(&os_name);
	int pkg_count = get_pkg_count();
	struct mem_info mi = get_mem_info();

	get_kernel_name(kernel_name);
	get_names(username, hostname);

	print_logo(os);
	printf("\x1b[5A\r\x1b[12C" PCOL "%s" SCOL "@" PCOL "%s", username, hostname);
	printf("\x1b[1B\r\x1b[12C" PCOL "os		" SCOL "%s", os_name);
	printf("\x1b[1B\r\x1b[12C" PCOL "kernel	" SCOL "%s", kernel_name);
	printf("\x1b[1B\r\x1b[12C" PCOL "pkgs	" SCOL "%d", pkg_count);
	printf("\x1b[1B\r\x1b[12C" PCOL "memory	" SCOL "%dM / %dM", mi.mem_used/1000, mi.mem_total/1000);
	printf("\n\n");

	return 0;
}
