#include "paths.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char horizon_home[HORIZON_PATH_MAX] = ".";

extern char enddbdir[];

static void strip_last_component(char *path)
{
	char *slash = strrchr(path, '/');
	if (slash == NULL)
		strcpy(path, ".");
	else if (slash == path)
		path[1] = '\0';
	else
		*slash = '\0';
}

void init_paths(void)
{
	const char *env = getenv("HORIZON_HOME");
	if (env != NULL && env[0] != '\0')
		snprintf(horizon_home, sizeof horizon_home, "%s", env);
	else
	{
		char exe[HORIZON_PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", exe, sizeof exe - 1);
		if (len > 0)
		{
			exe[len] = '\0';
			strip_last_component(exe);
			strip_last_component(exe);
			snprintf(horizon_home, sizeof horizon_home, "%s", exe);
		}
	}
	snprintf(enddbdir, HORIZON_PATH_MAX, "%s/Endgame/Endgame/cpr/", horizon_home);
}
