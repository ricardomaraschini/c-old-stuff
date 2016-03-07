#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <jansson.h>

typedef struct host {
	int	 host_id;
	char	*host_name;
	char	*host_alias;
} host;

struct hostdescr {
	char	*name;
	int	 type;
	size_t	 offset;
} hostdescr;


struct hostdescr hd[] = {
	{ "host_id", JSON_INTEGER, offsetof(host, host_id) },
	{ "host_name", JSON_STRING, offsetof(host, host_name) },
	{ "host_alias", JSON_STRING, offsetof(host, host_alias) }
};


int
parse_json(host *h, char *json)
{
	json_t			 *root;
	json_t			 *obj;
	json_error_t		  error;
	void			 *addr;
	int			  i;
	struct hostdescr	 *lhd;
	char			**aux;
	const char		 *strvalue;

	root = json_load_file(json, 0, &error);
	if (!root) {
		printf("%s\n", error.text);
		json_decref(root);
		return -1;
	}
	
	
	addr = (void *)h;
	for(i=0; i<3; i++) {
		lhd = &hd[i];
		
		obj = json_object_get(
		    root,
		    lhd->name
		);
		

		if (obj == NULL)
			continue;

		if (json_typeof(obj) != lhd->type)
			continue;

		switch(lhd->type) {
		
		case JSON_INTEGER:
			*(int *)(addr + lhd->offset) = json_integer_value(obj);
			break;

		case JSON_STRING:
			strvalue = json_string_value(obj);
			aux = (addr + lhd->offset);
			*aux = calloc(strlen(strvalue) + 1, sizeof(char));
			strcpy(*aux, strvalue);
			break;

		}

	}

	json_decref(root);

	return 0;
}


int
main(void)
{
	host	*h;
	char	 jsonfile[] = "json";

	h = calloc(1, sizeof(host));
	if (parse_json(h, jsonfile) != 0) {
		goto out;
	}


	printf("%s\n", h->host_name);

	out:
	free(h->host_name);
	free(h->host_alias);
	free(h);

	return 0;

}
