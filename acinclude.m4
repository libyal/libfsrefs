dnl Function to detect if libfsrefs dependencies are available
AC_DEFUN([AX_LIBFSREFS_CHECK_LOCAL],
 [dnl Check for internationalization functions in libfsrefs/libfsrefs_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

