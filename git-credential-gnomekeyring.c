#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gnome-keyring.h>

static GnomeKeyringPasswordSchema git_schema = {
    GNOME_KEYRING_ITEM_GENERIC_SECRET,
    {
        { "protocol", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING },
        { "host", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING },
        { "path", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING },
        { "username", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING },
        { NULL, 0 },
    }
};

typedef struct git_credential {
    gchar *protocol;
    gchar *host;
    gchar *path;
    gchar *username;
    gchar *password;
} git_credential_t;

static void
error(const char *err, ...)
{
    char msg[4096];
    va_list params;

    va_start(params, err);
    vsnprintf(msg, sizeof(msg), err, params);
    fprintf(stderr, "%s\n", msg);
    va_end(params);
}

static void
get_password(git_credential_t *cred)
{
    GnomeKeyringAttributeList *attrs;
    GnomeKeyringResult keyres;
    GList *found;
    int i;
    
    attrs = gnome_keyring_attribute_list_new();
    gnome_keyring_attribute_list_append_string(attrs, "protocol", cred->protocol);
    gnome_keyring_attribute_list_append_string(attrs, "host", cred->host);
    if (cred->path) {
	gnome_keyring_attribute_list_append_string(attrs, "path", cred->path);
    }
    if (cred->username) {
	gnome_keyring_attribute_list_append_string(attrs, "username", cred->username);
    }
    
    keyres = gnome_keyring_find_items_sync(git_schema.item_type, attrs, &found);
    gnome_keyring_attribute_list_free(attrs);
    if (keyres != GNOME_KEYRING_RESULT_OK || !found) {
	return;
    }
    
    g_printf("password=%s\n", ((GnomeKeyringFound *)found->data)->secret);
    attrs = ((GnomeKeyringFound *)found->data)->attributes;
    for (i = 0; i < attrs->len; ++i) {
	GnomeKeyringAttribute *attr = &gnome_keyring_attribute_list_index(attrs, i);
	if (attr->type == GNOME_KEYRING_ATTRIBUTE_TYPE_STRING &&
	    !strcmp(attr->name, "username")) {
	    g_printf("username=%s\n", attr->value.string);
	}
    }
    gnome_keyring_found_list_free(found);
}

static void
store_password(git_credential_t *cred)
{
    gchar desc[1024];
    GnomeKeyringResult keyres;

    /* Only store complete credentials */
    if (!cred->protocol || !cred->host ||
       	!cred->username || !cred->password)
	return;

    g_snprintf(desc, sizeof(desc), "Git %s", cred->host);
    keyres = gnome_keyring_store_password_sync(&git_schema,
					       GNOME_KEYRING_DEFAULT,
					       desc,
					       cred->password,
					       "protocol", cred->protocol,
					       "host", cred->host,
					       "path", cred->path,
					       "username", cred->username,
					       NULL);
    if (keyres != GNOME_KEYRING_RESULT_OK) {
	error("failed to store password");
	return;
    }
}

static void
erase_password(git_credential_t *cred)
{
    GnomeKeyringResult keyres;

    keyres = gnome_keyring_delete_password_sync(&git_schema,
						"protocol", cred->protocol,
						"host", cred->host,
						"path", cred->path,
						"username", cred->username,
						NULL);
    if (keyres != GNOME_KEYRING_RESULT_OK) {
	error("failed to delete password");
	return;
    }
}

static int
read_credential(git_credential_t *cred)
{
    char buf[1024];

    while (fgets(buf, sizeof(buf), stdin)) {
	char *v;

	if (strcmp(buf, "\n") == 0)
	    break;

	buf[strlen(buf) - 1] = '\0';
	v = strchr(buf, '=');
	if (!v) {
	    error("bad input: %s", buf);
	    return -1;
	}
	*v++ = '\0';

#define SET_CRED_ATTR(name) do { \
    if (strcmp(buf, #name) == 0) { \
	cred->name = g_strdup(v); \
    } \
} while (0)
	SET_CRED_ATTR(protocol);
	SET_CRED_ATTR(host);
	SET_CRED_ATTR(path);
	SET_CRED_ATTR(username);
	SET_CRED_ATTR(password);
    }
    return 0;
}

static void
clear_credential(git_credential_t *cred)
{
    if (cred->protocol) g_free(cred->protocol);
    if (cred->host) g_free(cred->host);
    if (cred->path) g_free(cred->path);
    if (cred->username) g_free(cred->username);
    if (cred->password) gnome_keyring_free_password(cred->password);
}

int
main(int argc, const char **argv)
{
    git_credential_t cred = {0};

    if (argc < 2) {
	error("Usage: git credential-gnomekeyring <get|store|erase>");
	return 1;
    }

    if (read_credential(&cred)) {
	clear_credential(&cred);
	return 1;
    }

    if (strcmp(argv[1], "get") == 0) {
	get_password(&cred);
    }
    else if (strcmp(argv[1], "store") == 0) {
	store_password(&cred);
    }
    else if (strcmp(argv[1], "erase") == 0) {
	erase_password(&cred);
    }
    clear_credential(&cred);

    return 0;
}
