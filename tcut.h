/* tcut.h: Tiny C Unit Test framework*/
#ifndef _TCUT_H
#define _TCUT_H

#define tcut_assert(what, test) do { if (!(test)) return what; } while (0)
#define tcut_run_test(test) do { char *what = test(); nr_tests++; if (what) return what; } while (0)
extern int nr_tests;

#endif