#include <string.h>

/* memcpy() and memset() are in core.c */

int strcmp(const char *s1, const char *s2)
{

	// Remarks:
	// 1) If we made it to the end of both strings (i. e. our pointer points to a
	//    '\0' character), the function will return 0
	// 2) If we didn't make it to the end of both strings, the function will
	//    return the difference of the characters at the first index of
	//    indifference.
	while ((*s1) && (*s1 == *s2)) {
		++s1;
		++s2;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

size_t strlen(const char *s)
{
	size_t len = 0;
	while (*s++) {
		len++;
	}
	return len;
}

char *strtok(char * restrict s1, const char * restrict s2)
{
	static char *tok_tmp = NULL;
	const char *p = s2;

	//new string
	if (s1 != NULL) {
		tok_tmp = s1;
	}
	//old string cont'd
	else {
		if (tok_tmp == NULL) {
			return NULL;
		}
		s1 = tok_tmp;
	}

	//skip leading s2 characters
	while (*p && *s1) {
		if (*s1 == *p) {
			++s1;
			p = s2;
			continue;
		}
		++p;
	}

	//no more to parse
	if (!*s1) {
		return (tok_tmp = NULL);
	}
	//skip non-s2 characters
	tok_tmp = s1;
	while (*tok_tmp) {
		p = s2;
		while (*p) {
			if (*tok_tmp == *p++) {
				*tok_tmp++ = '\0';
				return s1;
			}
		}
		++tok_tmp;
	}

	//end of string
	tok_tmp = NULL;
	return s1;
}

char* strcpy(char *dest, const char *src) {
    char* original_dest = dest;  // Remember the start of the destination
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;  // Return the start of the destination
}

char* strcat(char *dest, const char *src) {
    char* original_dest = dest;  // Remember the start of the destination
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;  // Return the start of the destination
}

// Function to remove trailing whitespace from a string
void rtrim(char *str) {
    int n = strlen(str) - 1;
    while (n >= 0 && (str[n] == ' ' || str[n] == '\t' || str[n] == '\n' || str[n] == '\r')) {
        str[n--] = '\0';
    }
}


char* strncpy(char *dst, const char *src, size_t n) {
    size_t i;

    // Copy characters from 'src' to 'dst' for up to 'n' characters or until a null terminator is found
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }

    // If the loop ends before 'n' iterations (because of a null terminator in 'src'),
    // fill the rest of 'dst' up to 'n' characters with null bytes
    for (; i < n; i++) {
        dst[i] = '\0';
    }

    return dst;
}
