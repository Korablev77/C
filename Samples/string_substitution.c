
/* Function is used for modifying SQL statement, which creates table or trigger.
 * This routine finds old table name in statement and replaces it
 * with the new one. Lengths of stings are necessary for the reason that
 * strings of SQL statement and name, which come from _space
 * are not null-terminated. If original statement contains table name without
 * quotes, then add them. It is requited due to further use of this statement
 * in callback function, which creates table.
 *
 * It is passed six arguments:
 *
 *   1) The complete text of the CREATE TABLE statement being modified, and its length.
 *   2) The old name of the table being renamed, and its length.
 *   3) The new name of the table being renamed, ant its length.
 *
 * Example of usage (lengths are omitted):
 * replace_table_name('CREATE TABLE abc(a, b, c)', 'abc', 'def')
 * 	-> 'CREATE TABLE "def"(a, b, c)'
 *
 * replace_table_name('CREATE TABLE abc(id, b, FOREIGN KEY(b) REFERENCES t2(id))',
 * 		      't2', 'def')
 * 	-> 'CREATE TABLE abc(id, b, FOREIGN KEY(b) REFERENCES "def"(id)'
 */
static uint32_t
replace_table_name(char* create_stmt, uint32_t create_stmt_len,
		   const char* old_name, uint32_t old_name_len,
		   const char* new_name, uint32_t new_name_len)
{
	assert(create_stmt);
	assert(old_name);
	assert(new_name);
	assert(create_stmt_len > old_name_len);

	char *ptr = create_stmt;
	char *sub_ptr = (char*) old_name;
	uint8_t is_quoted;
	uint32_t i, j;
	for (i = 0; i <= (create_stmt_len - old_name_len); i++) {
		while (i < (create_stmt_len - old_name_len)
		       && *(ptr+i) != ' ') i++;
		i++;
		is_quoted = 0;
		if (*(ptr+i) == '\"') {
			is_quoted = 1;
			i++;
		}
		for (j = 0; j < old_name_len; j++) {
			/*
			 * SQL statement which creates table, is held in _space
			 * table as it was entered by user, without
			 * any modification. In contrast, table name to be
			 * changed will come in upper-case, if it isn't quoted.
			 */
			if(is_quoted) {
				if (*(ptr+i+j) != *(sub_ptr+j)) break;
			} else {
				if (sqlite3Toupper(*(ptr+i+j)) != *(sub_ptr+j)) break;
			}
		}
		/*
		 * Make sure that it is not a substring:
		 * the next symbol has to be '(', '"' or ' ',
		 * which are the only tokens allowed to be after table name.
		 * Then construct new statement in temporary buffer,
		 * add quotes, if necessary and copy it back.
		 */
		if (j == old_name_len && (*(ptr+i+j) == '(' ||
		    *(ptr+i+j) == '\"'|| *(ptr+i+j) == ' ')) {

			char* temp_buf = (char*)region_alloc(&fiber()->gc,
							     create_stmt_len +
							     new_name_len + 2);
			uint8_t quotes = 0;
			memcpy(temp_buf, create_stmt, i);
			if (!is_quoted) {
				temp_buf[i] =  '\"';
				quotes++;
			}
			memcpy(temp_buf + i + quotes, new_name, new_name_len);
			if (!is_quoted) {
				temp_buf[i+new_name_len+quotes] = '\"';
				quotes++;
			}
			uint32_t full_len = create_stmt_len + new_name_len -
					    old_name_len + quotes;
			memcpy(temp_buf + i + new_name_len + quotes,
			       create_stmt + i + old_name_len,
			       create_stmt_len - i - old_name_len);
			memcpy(create_stmt, temp_buf, full_len);
			create_stmt[full_len] = '\0';
			return full_len;
		}
	}
	create_stmt[create_stmt_len] = '\0';
	return 0;
}

