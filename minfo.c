#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ddcChars.h>

//#define CS_BLOCK "@"

#define CWHITE "\x1b[38;2;255;255;255m"
#define CBLACK "\x1b[38;2;0;0;0m"
#define CRED "\x1b[38;2;255;0;0m"
#define CGREEN "\x1b[38;2;0;255;0m"
#define CBLUE "\x1b[38;2;0;0;255m"
#define CYELLOW "\x1b[38;2;255;255;0m"

#define OS_ARTIX 7599908386896181248
#define OS_ARCH	7163059223041243136

#define PCOL CYELLOW
#define SCOL CWHITE
#define GCOL CRED

const char* arch_linux =
GCOL
"      /\\      \n"
"     /  \\     \n"
"    /\\   \\    \n"
"   /      \\   \n"
"  /   /\\  -\\  \n"
" / _-'  '-_ \\ \n"
"/_/        \\_\\\n";

const char* artix_linux =
GCOL
"      /\\      \n"
"     /  \\     \n"
"    /-_  \\    \n"
"   /   `-_\\   \n"
"  /      _-\\  \n"
" / ___--'-_ \\ \n"
"/_/        \\_\\\n";

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

void get_cpu(char time[1024])
{
	int len;
	FILE *fp;
	fp = popen("cat /proc/cpuinfo", "r");
	if (fp == NULL) exit(1);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);

	fscanf(fp, "%s", time);
	len = strlen(time);
	while (*(long*)&time[len-8] != *(long*)"stepping")
	{
		time[len++] = ' ';
		fscanf(fp, "%s", &time[strlen(time)]);
		len = strlen(time);
	}
	time[len-8] = 0;
	pclose(fp);
}
void get_uptime(char time[1024])
{
	int len;
	FILE *fp;
	fp = popen("uptime -p", "r");
	if (fp == NULL) exit(1);
	fscanf(fp, "%s", time);
	fscanf(fp, "%s", time);
	len = strlen(time);
	time[len++] = ' ';
	fscanf(fp, "%s", &time[strlen(time)]);
	len = strlen(time);
	time[len++] = ' ';
	fscanf(fp, "%s", &time[strlen(time)]);
	len = strlen(time);
	time[len++] = ' ';
	fscanf(fp, "%s", &time[strlen(time)]);
	len = strlen(time);
	time[len++] = ' ';
	fscanf(fp, "%s", &time[strlen(time)]);
	len = strlen(time);
	time[len++] = ' ';
	fscanf(fp, "%s", &time[strlen(time)]);
	len = strlen(time);
	time[len++] = ' ';
	fscanf(fp, "%s", &time[strlen(time)]);
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
	char uptime[1024] = {0};
	char cpuinfo[1024] = {0};
	long os = get_os(&os_name);
	//int pkg_count = get_pkg_count();
	struct mem_info mi = get_mem_info();

	get_kernel_name(kernel_name);
	get_names(username, hostname);
	get_uptime(uptime);
	get_cpu(cpuinfo);

	print_logo(os);
	printf( 
"\x1b[7A\r\x1b[16C"	PCOL "%s" SCOL "@" PCOL "%s"
"\x1b[1B\r\x1b[16C"	PCOL "os	" SCOL "%s"
"\x1b[1B\r\x1b[16C"	PCOL "kernel	" SCOL "%s"
//"\x1b[1B\r\x1b[16C"	PCOL "pkgs	" SCOL "%d"
"\x1b[1B\r\x1b[16C"	PCOL "uptime	" SCOL "%s"
"\x1b[1B\r\x1b[16C"	PCOL "cpu	" SCOL "%s"
"\x1b[1B\r\x1b[16C"	PCOL "memory	" SCOL "%dM / %dM"
"\x1b[1B\r\x1b[16C"	//"\x1b[38;5;0m" CS_BLOCK
			"\x1b[38;5;1m" CS_BLOCK
			"\x1b[38;5;2m" CS_BLOCK
			"\x1b[38;5;3m" CS_BLOCK
			"\x1b[38;5;4m" CS_BLOCK
			"\x1b[38;5;5m" CS_BLOCK
			"\x1b[38;5;6m" CS_BLOCK
			"\x1b[38;5;7m" CS_BLOCK
//"\x1b[1B\r\x1b[12C"
			//"\x1b[38;5;8m" CS_BLOCK
			"\x1b[38;5;9m" CS_BLOCK
			"\x1b[38;5;10m" CS_BLOCK
			"\x1b[38;5;11m" CS_BLOCK
			"\x1b[38;5;12m" CS_BLOCK
			"\x1b[38;5;13m" CS_BLOCK
			"\x1b[38;5;14m" CS_BLOCK
			"\x1b[38;5;15m" CS_BLOCK
		"\n\n",
		username, hostname, 
		os_name, 
		kernel_name, 
		//pkg_count, 
		uptime, 
		cpuinfo, 
		mi.mem_used/1000, mi.mem_total/1000);

	return 0;
}
