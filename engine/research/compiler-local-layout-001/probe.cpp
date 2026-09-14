#ifdef __cplusplus
extern "C" {
#endif
int probe_product(int input) {
 int first, second;
 first=input+3; second=input*7; return first-second;
}
int probe_decl_order(int input) {
 int second, first;
 first=input+3; second=input*7; return first-second;
}
int probe_scope(int input) {
 int first=input+3;
 { int second=input*7; return first-second; }
}
int probe_three(int input) {
 int retained, steps, total;
 retained=input*3; steps=input+5; total=retained-steps; return total;
}
int probe_names(int input) {
 int prior, current;
 prior=input+3; current=input*7; return prior-current;
}
typedef struct PROBE_ROW {int first,second,third;} PROBE_ROW;
extern PROBE_ROW probe_rows[];
int probe_pointer(int input) {
 PROBE_ROW *row;
 row=&probe_rows[input]; return row->first;
}
#ifdef __cplusplus
}
#endif
