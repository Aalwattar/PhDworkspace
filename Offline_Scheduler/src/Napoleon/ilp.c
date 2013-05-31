/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : May 22, 2013
 *************************************************************************/

//global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

//local header files
#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <ilp.h>
#include <main.h>

void ilp_equations(FILE *, t_task *, int, int *, int *);
void print_first_constraint(FILE*, t_task*, int);
void print_second_constraint(FILE*, t_task*, int);
void print_third_constraint(FILE*, t_task*, int);
void print_fourth_constraint(FILE*, t_task*);
//void print_fifth_constraint(FILE*, t_task*,   int);
void print_sixth_constraint(FILE*, t_task*, int);
void print_seventh_constraint(FILE*, t_task*, int);
void print_eighth_constraint(FILE*, t_task*, int);
void print_nineth_constraint(FILE*, t_task*, int*);
void print_tenth_constraint(FILE*, t_task*, int);
void print_eleventh_constraint(FILE*, t_task*, int);
void print_twelveth_constraint(FILE*, t_task*, int);
void print_thirteenth_constraint(FILE*, t_task*, int, int*);
//void print_fourteenth_constraint(FILE*, t_task*,   int);
void print_fifteenth_constraint(FILE*, t_task*, int);
void print_sixteenth_constraint(FILE*, t_task*, int);


int calc_T(t_task *task, int *T){
    int i = 0;
    int num_nodes = task->width;
    *T = 0;

    for (i = 1; i <= num_nodes; i++) {
        *T = *T + (task + i)->latency + (task + i)->reconfig_time;
    }
    fprintf(stderr, "UPPER BOUND EXECUTION TIME IS %d\n", *T);

    return *T;
}


void ilp_equations(FILE *ilp_strm, t_task *task, int T, int *succ_adj_mat, int *reuse_mat){
    print_first_constraint(ilp_strm, task, T);
    print_second_constraint(ilp_strm, task, T);
    print_third_constraint(ilp_strm, task, T);
    print_fourth_constraint(ilp_strm, task);
    //print_fifth_constraint(ilp_strm, task, T);
    print_sixth_constraint(ilp_strm, task, T);
    print_seventh_constraint(ilp_strm, task, T);
    print_eighth_constraint(ilp_strm, task, T);
    print_nineth_constraint(ilp_strm, task, succ_adj_mat);
    print_tenth_constraint(ilp_strm, task, T);
    print_eleventh_constraint(ilp_strm, task, T);
    print_twelveth_constraint(ilp_strm, task, T);
    print_thirteenth_constraint(ilp_strm, task, T, reuse_mat);
    //print_fourteenth_constraint(ilp_strm, task, T);
    print_fifteenth_constraint(ilp_strm, task, T);
    print_sixteenth_constraint(ilp_strm, task, T);
}


void print_second_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, m, n, l, j;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "First Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        for (h = 0; h <= T; h++) {
            for (n = 0; n <= T; n++) {
                for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                    for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                        fprintf(ilp_strm, "P%d%d%d%d ", i, h, k, m);
                        for (l = 1; l <= __NUM_FPGA_COLUMNS; l++) {
                            for (j = 1; j <= __NUM_FPGA_ROWS; j++) {
                                if (l != k || j != m)
                                    fprintf(ilp_strm, "+ P%d%d%d%d ", i, n, l, j);
                            }
                        }
                        fprintf(ilp_strm, "<= 1\n");
                    }
                }
            }
        }
    }

}


void print_first_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, m, l, l_min, j, j_min;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Second Constraint Equations\n");

    fprintf(ilp_strm, "minimize\n");
    fprintf(ilp_strm, "te\n");
    fprintf(ilp_strm, "subject to\n");

    for (h = 0; h <= T; h++) {
        for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
            for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                for (i = 1; i <= num_nodes; i++) {
                    l_min = k - (task + i)->columns + 1;
                    j_min = m - (task + i)->rows + 1;
                    if (l_min < 1) l_min = 1;
                    if (j_min < 1) j_min = 1;
                    for (l = l_min; l <= k; l++) {
                        for (j = j_min; j <= m; j++) {
                            fprintf(ilp_strm, "+ P%d%d%d%d ", i, h, l, j);
                        }
                    }
                }
                fprintf(ilp_strm, "<= 1\n");
            }
        }
    }

}


void print_third_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, k_min, m, m_min;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Third Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        k_min = __NUM_FPGA_COLUMNS - (task + i)->columns + 2;
        m_min = __NUM_FPGA_ROWS - (task + i)->rows + 2;
        for (h = 0; h <= T; h++) {
            for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                    if (k >= k_min || m >= m_min)
                        fprintf(ilp_strm, "+ P%d%d%d%d ", i, h, k, m);
                }
            }
        }
    }
    fprintf(ilp_strm, "= 0\n");

}


void print_fourth_constraint(FILE *ilp_strm, t_task *task) {
    int i, k, m;
    int num_nodes = task->width;

    for (i = 1; i <= num_nodes; i++) {
        for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
            for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                fprintf(ilp_strm, "+ P%d0%d%d ", i, k, m);
            }
        }
    }

    fprintf(ilp_strm, "= 0\n");
}


void print_sixth_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, m;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Sixth Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        for (h = 0; h <= T; h++) {
            fprintf(ilp_strm, "Son%d ", i);
            for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                    if (h != T)
                        fprintf(ilp_strm, "+ %d P%d%d%d%d ", (T - h), i, h, k, m);
                }
            }
            fprintf(ilp_strm, "<= %d\n", T);
        }
    }

}


void print_seventh_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, m;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Seventh Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        for (h = 0; h <= T; h++) {
            fprintf(ilp_strm, "- Soff%d ", i);
            for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                    fprintf(ilp_strm, "+ %d P%d%d%d%d ", h, i, h, k, m);
                }
            }
            fprintf(ilp_strm, "<= 0\n");
        }
    }

}


void print_eighth_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, m;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Eighth Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "Son%d - Soff%d ", i, i);
        for (h = 0; h <= T; h++) {
            for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                    fprintf(ilp_strm, "+ P%d%d%d%d ", i, h, k, m);
                }
            }
        }
        fprintf(ilp_strm, "= 1\n");
    }

}


void print_nineth_constraint(FILE *ilp_strm, t_task *task, int *succ_adj_mat) {
    int i, j;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Nineth Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        for (j = 1; j <= num_nodes; j++) {
            if (*(succ_adj_mat + (num_nodes + 2) * i + j) == 1) {
                fprintf(ilp_strm, "Soff%d - Soff%d >= %d\n", j, i, (task + j)->latency);
            }
        }
    }

}


void print_tenth_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, m;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Tenth Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "%d M%d ", (task + i)->reconfig_time, i);
        for (h = 0; h <= T; h++) {
            for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                    fprintf(ilp_strm, "+ P%d%d%d%d ", i, h, k, m);
                }
            }
        }
        fprintf(ilp_strm, ">= %d\n", (task + i)->reconfig_time + (task + i)->latency);
    }

}


void print_eleventh_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Eleventh Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "Son%d - %d M%d ", i, T, i);
        for (h = 1; h <= T; h++) {
            fprintf(ilp_strm, "- %d t%d%d ", h, i, h);
        }
        fprintf(ilp_strm, "<= 0\n");

        fprintf(ilp_strm, "Son%d + %d M%d ", i, T, i);
        for (h = 1; h <= T; h++) {
            fprintf(ilp_strm, "- %d t%d%d ", h, i, h);
        }
        fprintf(ilp_strm, ">= 0\n");
    }

}


void print_twelveth_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, m, m_min;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Tweleveth Constraint Equations\n");

    for (h = 0; h <= T; h++) {
        for (i = 1; i <= num_nodes; i++) {
            m_min = h - (task + i)->reconfig_time + 1;
            if (m_min < 1) m_min = 1;
            for (m = m_min; m <= h; m++) {
                fprintf(ilp_strm, "+ t%d%d ", i, m);
            }
        }
        fprintf(ilp_strm, "<= 1\n");
    }

}


void print_thirteenth_constraint(FILE *ilp_strm, t_task *task, int T, int *reuse_mat) {
    int i, h, k, m, j;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Thirteenth Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        for (h = 1; h <= T; h++) {
            for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                    fprintf(ilp_strm, "M%d + P%d%d%d%d ", i, i, h, k, m);
                    for (j = 1; j <= num_nodes; j++) {
                        if (*(reuse_mat + (num_nodes + 2) * i + j) == 1) {
                            fprintf(ilp_strm, "- P%d%d%d%d ", j, h - 1, k, m);
                        }
                    }
                    fprintf(ilp_strm, "<= 1\n");
                }
            }
        }
    }

}


void print_fifteenth_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Fifteenth Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "M%d ", i);
        for (h = 0; h <= T; h++) {
            fprintf(ilp_strm, "+ t%d%d ", i, h);
        }
        fprintf(ilp_strm, "= 1\n");
    }

}


void print_sixteenth_constraint(FILE *ilp_strm, t_task *task, int T) {
    int i, h, k, m;
    int num_nodes = task->width;

    //fprintf(ilp_strm, "/***********************************************************************************/\n");
    //fprintf(ilp_strm, "Sixteenth Constraint Equations\n");

    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "Soff%d - te <= 0\n", i);
    }

    //fprintf(ilp_strm, "te <= %d\n", T);

    fprintf(ilp_strm, "bounds\n");
    fprintf(ilp_strm, "0 < te <= %d\n", T);
    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "0 <= Son%d <= %d\n", i, T);
        fprintf(ilp_strm, "0 <= Soff%d <= %d\n", i, T);
    }
    fprintf(ilp_strm, "integer\n");
    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "Son%d\n", i);
        fprintf(ilp_strm, "Soff%d\n", i);
    }
    fprintf(ilp_strm, "te\n");
    fprintf(ilp_strm, "binary\n");
    for (i = 1; i <= num_nodes; i++) {
        for (h = 0; h <= T; h++) {
            for (k = 1; k <= __NUM_FPGA_COLUMNS; k++) {
                for (m = 1; m <= __NUM_FPGA_ROWS; m++) {
                    fprintf(ilp_strm, "P%d%d%d%d\n", i, h, k, m);
                }
            }
        }
    }
    for (i = 1; i <= num_nodes; i++) {
        for (h = 0; h <= T; h++) {
            fprintf(ilp_strm, "t%d%d\n", i, h);
        }
    }
    for (i = 1; i <= num_nodes; i++) {
        fprintf(ilp_strm, "M%d\n", i);
    }
    fprintf(ilp_strm, "end\n");
    fprintf(ilp_strm, "\n");
}


