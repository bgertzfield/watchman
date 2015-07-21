/* Copyright 1986-present, Rich $alz and Wayne Davison.
 * Licensed under the Apache License, Version 2.0 */

/* wildmatch.h */

int wildmatch(const char *pattern, const char *text);
int iwildmatch(const char *pattern, const char *text);
int wildmatch_array(const char *pattern, const char*const *texts, int where);
int litmatch_array(const char *string, const char*const *texts, int where);
