#include "../include/shared.h"

void datatype_token_to_str(char res[], int type) {
    switch (type) {
        case REF_SYMBOL:
            strcpy(res, "REF\0");
            break;

        case STR_SYMBOL:
            strcpy(res, "STR\0");
            break;

        case CON_SYMBOL:
            strcpy(res, "CON\0");
            break;

        case LIS_SYMBOL:
            strcpy(res, "LIS\0");
            break;

        default:
            break;
    }
}

void vartype_token_to_str(char res[], int type) {
    switch (type) {
        case BASIC_VAR:
            strcpy(res, "VAR\0");
            break;

        case SINGLETON_VAR:
            strcpy(res, "SINGLETON VAR\0");
            break;

        case ANONYM_VAR:
            strcpy(res, "ANONYMOUS VAR\0");
            break;

        default:
            break;
    }

}
