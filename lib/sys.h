#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/* ../src/runtime/c/array.c */
extern void set_array_header P_((CL_FORM **array_header, int header_size, int rank));
extern CL_FORM *make_pure_array P_((CL_FORM *base, int header_size, int rank, CL_FORM *dimensions, long element_type, long array_size));
extern void make_vector_internal P_((CL_FORM *base));
extern void make_array_internal P_((CL_FORM *base, int nargs));
extern void row_major_aref_internal P_((CL_FORM *base));
extern void set_row_major_aref_internal P_((CL_FORM *base));
extern void svref_internal P_((CL_FORM *base));
extern void set_svref_internal P_((CL_FORM *base));
extern void array_element_type_internal P_((CL_FORM *base));
extern void Farray_rank P_((CL_FORM *base));
extern void array_dimension_internal P_((CL_FORM *base));
extern void Farray_total_size P_((CL_FORM *base));
extern void shrink_vector P_((CL_FORM *base));
extern void Fadjustable_array_p P_((CL_FORM *base));
extern void Farray_has_fill_pointer_p P_((CL_FORM *base));
extern void Ffill_pointer P_((CL_FORM *base));
extern void set_fill_pointer_internal P_((CL_FORM *base));
extern void displace_array P_((CL_FORM *base));
/* ../src/runtime/c/catch.c */
extern void unwind_to P_((CL_FORM *saved_bind_top));
extern void catch_internal P_((CL_FORM *base));
extern void call_cont P_((CL_FORM *base));
extern void throw_internal P_((CL_FORM *base, int nargs));
extern void unwind_protect_internal P_((CL_FORM *base));
/* ../src/runtime/c/character.c */
extern char check_char P_((CL_FORM *base));
extern char save_toupper P_((int ch));
extern void Fstandard_char_p P_((CL_FORM *base));
extern void Fgraphic_char_p P_((CL_FORM *base));
extern void Fstring_char_p P_((CL_FORM *base));
extern void Falpha_char_p P_((CL_FORM *base));
extern void Fupper_case_p P_((CL_FORM *base));
extern void Flower_case_p P_((CL_FORM *base));
extern void Fboth_case_p P_((CL_FORM *base));
extern void digit_char_p_internal P_((CL_FORM *base));
extern void Falphanumericp P_((CL_FORM *base));
extern void FcharE P_((CL_FORM *base, int nargs));
extern void FcharNE P_((CL_FORM *base, int nargs));
extern void FcharL P_((CL_FORM *base, int nargs));
extern void FcharG P_((CL_FORM *base, int nargs));
extern void FcharLE P_((CL_FORM *base, int nargs));
extern void FcharGE P_((CL_FORM *base, int nargs));
extern void Fchar_equal P_((CL_FORM *base, int nargs));
extern void Fchar_not_equal P_((CL_FORM *base, int nargs));
extern void Fchar_lessp P_((CL_FORM *base, int nargs));
extern void Fchar_greaterp P_((CL_FORM *base, int nargs));
extern void Fchar_not_greaterp P_((CL_FORM *base, int nargs));
extern void Fchar_not_lessp P_((CL_FORM *base, int nargs));
extern void Fchar_code P_((CL_FORM *base));
extern void code_char_internal P_((CL_FORM *base));
extern void Fchar_upcase P_((CL_FORM *base));
extern void Fchar_downcase P_((CL_FORM *base));
extern void digit_char_internal P_((CL_FORM *base));
extern void Fchar_int P_((CL_FORM *base));
extern void Fchar_name P_((CL_FORM *base));
extern int our_strncasecmp P_((char *str1, char *str2, int len));
extern void Fname_char P_((CL_FORM *base));
/* ../src/runtime/c/clos.c */
extern void make_instance_internal P_((CL_FORM *base));
/* ../src/runtime/c/debug.c */
extern void inspect P_((CL_FORM *base, int level, int length, int in_list_p));
extern void stack_cont P_((CL_FORM *base, int offset, int nargs));
extern void dc_pure_symbol P_((CL_FORM *symbol));
extern void symbol_module_i P_((CL_FORM *sym_base, int i));
extern void show_alist P_((CL_FORM *base, int i));
/* ../src/runtime/c/environ.c */
extern void c_getenv P_((CL_FORM *base));
extern void c_system P_((CL_FORM *base));
extern void c_argc P_((CL_FORM *base));
extern void c_argv P_((CL_FORM *base));
/* ../src/runtime/c/file.c */
extern void c_fopen P_((CL_FORM *base));
extern void c_fclose P_((CL_FORM *base));
extern void c_ftell P_((CL_FORM *base));
extern void c_fseek P_((CL_FORM *base));
extern void c_file_length P_((CL_FORM *base));
extern void c_stdin P_((CL_FORM *base));
extern void c_stdout P_((CL_FORM *base));
extern void c_fgetc P_((CL_FORM *base));
extern void c_fputc P_((CL_FORM *base));
extern void c_ungetc P_((CL_FORM *base));
/* ../src/runtime/c/foreign.c */
extern void internal_make_c_struct P_((CL_FORM *base));
extern void internal_c_struct_p P_((CL_FORM *base));
extern void internal_make_c_struct_ptr P_((CL_FORM *base));
extern void internal_c_struct_ptr_p P_((CL_FORM *base));
extern void internal_get_struct_pointer P_((CL_FORM *base));
extern void make_c_char P_((CL_FORM *base));
extern void make_c_short P_((CL_FORM *base));
extern void make_c_int P_((CL_FORM *base));
extern void make_c_long P_((CL_FORM *base));
extern void make_c_unsigned_char P_((CL_FORM *base));
extern void make_c_unsigned_short P_((CL_FORM *base));
extern void make_c_unsigned_int P_((CL_FORM *base));
extern void make_c_unsigned_long P_((CL_FORM *base));
extern void make_c_float P_((CL_FORM *base));
extern void make_c_double P_((CL_FORM *base));
extern void make_c_long_double P_((CL_FORM *base));
extern void make_lisp_character P_((CL_FORM *base));
extern void make_lisp_integer P_((CL_FORM *base));
extern void make_lisp_float P_((CL_FORM *base));
extern void make_lisp_string P_((CL_FORM *base));
extern void c_float_p P_((CL_FORM *base));
extern void c_double_p P_((CL_FORM *base));
extern void c_long_double_p P_((CL_FORM *base));
extern void make_c_char_ptr P_((CL_FORM *base));
extern void make_c_short_ptr P_((CL_FORM *base));
extern void make_c_int_ptr P_((CL_FORM *base));
extern void make_c_long_ptr P_((CL_FORM *base));
extern void make_c_unsigned_char_ptr P_((CL_FORM *base));
extern void make_c_unsigned_short_ptr P_((CL_FORM *base));
extern void make_c_unsigned_int_ptr P_((CL_FORM *base));
extern void make_c_unsigned_long_ptr P_((CL_FORM *base));
extern void c_char_ptr_p P_((CL_FORM *base));
extern void c_short_ptr_p P_((CL_FORM *base));
extern void c_int_ptr_p P_((CL_FORM *base));
extern void c_long_ptr_p P_((CL_FORM *base));
extern void c_unsigned_char_ptr_p P_((CL_FORM *base));
extern void c_unsigned_short_ptr_p P_((CL_FORM *base));
extern void c_unsigned_int_ptr_p P_((CL_FORM *base));
extern void c_unsigned_long_ptr_p P_((CL_FORM *base));
/* ../src/runtime/c/funcall.c */
extern void Ffuncall P_((CL_FORM *base, int nargs));
extern void Fapply P_((CL_FORM *base, int nargs));
/* ../src/runtime/c/hash.c */
extern unsigned long hash P_((char *str, long len));
extern void string_hash P_((CL_FORM *base));
extern void sxhash_simple_string P_((CL_FORM *base));
extern void sxhash_string P_((CL_FORM *base));
extern void combine_hash P_((CL_FORM *base));
/* ../src/runtime/c/keysort.c */
extern void init_keysort P_((CL_FORM *base));
extern void keysort P_((CL_FORM *first_arg, int nargs, int nkey, CL_FORM *keylist[], char suppl_flags[], int allow_other));
/* ../src/runtime/c/list.c */
extern CL_FORM *NEXT_CAR P_((CL_FORM *lptr));
extern long list_length P_((CL_FORM *lptr));
extern void Flist P_((CL_FORM *base, int nargs));
extern void FlistX P_((CL_FORM *base, int nargs));
extern void Fappend P_((CL_FORM *base, int nargs));
extern void raw_list_length P_((CL_FORM *base));
extern void simple_assoc P_((CL_FORM *base));
/* ../src/runtime/c/main.c */
extern int main P_((int argc, char *argv[]));
/* ../src/runtime/c/number.c */
extern double get_float P_((CL_FORM *base));
extern double *make_float P_((CL_FORM *base, double num_float));
extern void Fzerop P_((CL_FORM *base));
extern void Fplusp P_((CL_FORM *base));
extern void Fminusp P_((CL_FORM *base));
extern void Foddp P_((CL_FORM *base));
extern void Fevenp P_((CL_FORM *base));
extern void Fnumeql P_((CL_FORM *base, int nargs));
extern void Fnumneql P_((CL_FORM *base, int nargs));
extern void Flt P_((CL_FORM *base, int nargs));
extern void Fgt P_((CL_FORM *base, int nargs));
extern void Fle P_((CL_FORM *base, int nargs));
extern void Fge P_((CL_FORM *base, int nargs));
extern void Fplus P_((CL_FORM *base, int nargs));
extern void Fminus P_((CL_FORM *base, int nargs));
extern void Fmult P_((CL_FORM *base, int nargs));
extern void Fdiv P_((CL_FORM *base, int nargs));
extern void F1plus P_((CL_FORM *base));
extern void F1minus P_((CL_FORM *base));
extern void expt_internal P_((CL_FORM *base));
extern void log_internal P_((CL_FORM *base));
extern void Fsqrt P_((CL_FORM *base));
extern void Fsin P_((CL_FORM *base));
extern void Fcos P_((CL_FORM *base));
extern void Ftan P_((CL_FORM *base));
extern void Fasin P_((CL_FORM *base));
extern void Facos P_((CL_FORM *base));
extern void Fatan P_((CL_FORM *base));
extern void float_internal P_((CL_FORM *base));
extern void convert_to_int P_((CL_FORM *base, int ctype));
extern void floor_internal P_((CL_FORM *base));
extern void ceiling_internal P_((CL_FORM *base));
extern void truncate_internal P_((CL_FORM *base));
extern void round_internal P_((CL_FORM *base));
extern void Finteger_length P_((CL_FORM *base));
/* ../src/runtime/c/obrep1.c */
extern void save_form P_((CL_FORM *form));
/* ../src/runtime/c/obrep2.c */
/* ../src/runtime/c/progv.c */
extern void progv_internal P_((CL_FORM *base));
/* ../src/runtime/c/string.c */
extern char *get_c_string P_((CL_FORM *lisp_string));
extern char *sm_get_c_string P_((CL_FORM *lisp_string));
extern void make_string P_((CL_FORM *base, char *string));
extern void char_internal P_((CL_FORM *base));
extern void set_char_internal P_((CL_FORM *base));
extern void schar_internal P_((CL_FORM *base));
extern void set_schar_internal P_((CL_FORM *base));
extern void make_string_internal P_((CL_FORM *base));
/* ../src/runtime/c/structure.c */
extern void structp P_((CL_FORM *base));
extern void struct_type P_((CL_FORM *base));
extern void new_struct P_((CL_FORM *base));
extern void struct_size P_((CL_FORM *base));
extern void struct_ref_internal P_((CL_FORM *base));
extern void set_struct_ref_internal P_((CL_FORM *base));
/* ../src/runtime/c/symbols.c */
extern void Fsymbol_value P_((CL_FORM *base));
extern void Fboundp P_((CL_FORM *base));
extern void unbound_value_p P_((CL_FORM *base));
extern void Fset P_((CL_FORM *base));
extern void Fmakunbound P_((CL_FORM *base));
extern void Fsymbol_plist P_((CL_FORM *base));
extern void Fset_symbol_plist P_((CL_FORM *base));
extern void Fsymbol_name P_((CL_FORM *base));
extern void Fmake_symbol P_((CL_FORM *base));
extern void Fsymbol_package P_((CL_FORM *base));
extern void symbol_package_index P_((CL_FORM *base));
extern void set_symbol_package P_((CL_FORM *base));
extern void set_constant_flag P_((CL_FORM *base));
extern void setup_symbols_iterator P_((CL_FORM *base));
/* ../src/runtime/c/system.c */
extern void memallot P_((void));
extern CL_FORM *form_swap P_((CL_FORM *from, long num));
extern long *fx_swap P_((long *from, long num));
extern char *ch_swap P_((char *from, long num));
extern double *fl_swap P_((double *from, long num));
extern void gc P_((CL_FORM *base));
extern void gc_symbols P_((CL_INIT *symbol));
extern long *fixnum_alloc P_((CL_FORM *base, long num));
extern char *char_alloc P_((CL_FORM *base, unsigned long rnum));
extern double *float_alloc P_((CL_FORM *base, long num));
extern CL_FORM *form_alloc P_((CL_FORM *base, long num));
extern long *bits_alloc P_((CL_FORM *base, long num));
extern void Labort P_((char *msg));
extern void Lerror P_((CL_FORM *base, char *msg));
extern void warning P_((char *warning_str));
/* ../src/runtime/c/unix.c */
extern void unix_current_directory P_((CL_FORM *base));
extern void unix_file_mode P_((CL_FORM *base));
extern void unix_link_mode P_((CL_FORM *base));
extern void unix_readlink P_((CL_FORM *base));
/* ../src/runtime/c/values.c */
extern void Fvalues P_((CL_FORM *base, int nargs));
extern void Fvalues_list P_((CL_FORM *base));
extern void save_values P_((CL_FORM *base));

#undef P_
