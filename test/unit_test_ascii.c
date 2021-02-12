#include <inttypes.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h> // TODO need this?
#include "unit_test.h"
#include "slow5.h"
#include "slow5_extra.h"
#include "slow5idx.h"
#include "misc.h"

int slow5_open_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);
    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_open_null(void) {
    // Pathname NULL
    struct slow5_file *s5p = slow5_open(NULL, "r");
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    // Mode NULL
    s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", NULL);
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    // Both NULL
    s5p = slow5_open(NULL, NULL);
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    return EXIT_SUCCESS;
}

int slow5_open_invalid(void) {
    // Path invalid
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5_invalid/exp_1_default.slow5", "r");
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    // Extension invalid
    s5p = slow5_open("test/data/err/one_fast5/invalid_extension.slow", "r");
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    return EXIT_SUCCESS;
}

int slow5_open_with_valid(void) {
    struct slow5_file *s5p = slow5_open_with("test/data/exp/one_fast5/exp_1_default.slow5", "r", FORMAT_ASCII);
    ASSERT(s5p != NULL);
    ASSERT(slow5_close(s5p) == 0);

    s5p = slow5_open_with("test/data/err/one_fast5/invalid_extension.slow", "r", FORMAT_ASCII);
    ASSERT(s5p != NULL);
    ASSERT(slow5_close(s5p) == 0);

    s5p = slow5_open_with("test/data/exp/one_fast5/exp_1_default.slow5", "r", FORMAT_UNKNOWN);
    ASSERT(s5p != NULL);
    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_open_with_null(void) {
    // Pathname NULL
    struct slow5_file *s5p = slow5_open_with(NULL, "r", FORMAT_ASCII);
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    // Mode NULL
    s5p = slow5_open_with("test/data/exp/one_fast5/exp_1_default.slow5", NULL, FORMAT_BINARY);
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    // Both NULL
    s5p = slow5_open_with(NULL, NULL, FORMAT_UNKNOWN);
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    return EXIT_SUCCESS;
}

int slow5_open_with_invalid(void) {
    // Format invalid
    struct slow5_file *s5p = slow5_open_with("test/data/exp/one_fast5/exp_1_default.slow5", "r", FORMAT_BINARY);
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    // Extension invalid
    s5p = slow5_open_with("test/data/err/one_fast5/invalid_extension.slow", "r", FORMAT_UNKNOWN);
    ASSERT(s5p == NULL);
    ASSERT(slow5_close(s5p) == EOF);

    return EXIT_SUCCESS;
}

int slow5_get_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_get_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get(NULL, &read, s5p) == -1);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", NULL, s5p) == -1);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, NULL) == -1);
    ASSERT(slow5_get(NULL, NULL, s5p) == -1);
    ASSERT(slow5_get(NULL, &read, NULL) == -1);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", NULL, NULL) == -1);
    ASSERT(slow5_get(NULL, NULL, NULL) == -1);
    // Shouldn't need to slow5_rec_free

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_get_invalid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    // TODO simulate -2 error

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get("badreadid", &read, s5p) == -3);
    ASSERT(slow5_get("", &read, s5p) == -3);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be", &read, s5p) == -3);
    ASSERT(slow5_get("O_O", &read, s5p) == -3);

    ASSERT(slow5_close(s5p) == 0);

    // TODO simulate -4 error

    s5p = slow5_open("test/data/err/parse_bad.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == -5);
    ASSERT(slow5_get("1", &read, s5p) == -5);
    ASSERT(slow5_get("2", &read, s5p) == -5);
    ASSERT(slow5_get("3", &read, s5p) == -5);
    ASSERT(slow5_get("4", &read, s5p) == -5);
    ASSERT(slow5_get("5", &read, s5p) == -5);
    ASSERT(slow5_get("6", &read, s5p) == -5);
    ASSERT(slow5_get("7", &read, s5p) == -5);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_get_many_same(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    ASSERT(slow5_get("a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read, s5p) == 0);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_get_many_different(void) {
    struct slow5_file *s5p = slow5_open("test/data/test/same_diff_ids.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get("1", &read, s5p) == 0);
    ASSERT(slow5_get("2", &read, s5p) == 0);
    ASSERT(slow5_get("3", &read, s5p) == 0);
    ASSERT(slow5_get("4", &read, s5p) == 0);
    ASSERT(slow5_get("5", &read, s5p) == 0);
    ASSERT(slow5_get("6", &read, s5p) == 0);
    ASSERT(slow5_get("7", &read, s5p) == 0);
    ASSERT(slow5_get("8", &read, s5p) == 0);
    ASSERT(slow5_get("9", &read, s5p) == 0);
    ASSERT(slow5_get("10", &read, s5p) == 0);
    ASSERT(slow5_get("--", &read, s5p) == 0);
    ASSERT(slow5_get("", &read, s5p) == 0);
    ASSERT(slow5_get(".", &read, s5p) == 0);
    ASSERT(slow5_get("^_^", &read, s5p) == 0);
    ASSERT(slow5_get("lol", &read, s5p) == 0);
    ASSERT(slow5_get("weirdid", &read, s5p) == 0);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}


int slow5_get_next_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_get_next_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(NULL, s5p) == -1);
    ASSERT(slow5_get_next(&read, NULL) == -1);
    ASSERT(slow5_get_next(NULL, NULL) == -1);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_get_next_empty(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    ASSERT(slow5_get_next(&read, s5p) == -2);
    ASSERT(slow5_get_next(&read, s5p) == -2);
    ASSERT(slow5_get_next(&read, s5p) == -2);
    ASSERT(slow5_get_next(&read, s5p) == -2);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);

    return EXIT_SUCCESS;
}

int slow5_get_next_invalid(void) {
    struct slow5_file *s5p = slow5_open("test/data/err/parse_bad.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == -3);
    ASSERT(slow5_get_next(&read, s5p) == -3);
    ASSERT(slow5_get_next(&read, s5p) == -3);
    ASSERT(slow5_get_next(&read, s5p) == -3);
    ASSERT(slow5_get_next(&read, s5p) == -3);
    ASSERT(slow5_get_next(&read, s5p) == -3);
    ASSERT(slow5_get_next(&read, s5p) == -3);
    ASSERT(slow5_get_next(&read, s5p) == -3);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_to_mem_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    char *str = slow5_rec_to_mem(read, FORMAT_ASCII, s5p->compress, NULL);
    printf("%s", str);
    free(str);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_to_mem_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    size_t num_bytes;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    ASSERT(slow5_rec_to_mem(NULL, FORMAT_ASCII, s5p->compress, &num_bytes) == NULL);
    ASSERT(slow5_rec_to_mem(read, FORMAT_UNKNOWN, NULL, &num_bytes) == NULL);
    ASSERT(slow5_rec_to_mem(NULL, FORMAT_UNKNOWN, s5p->compress, &num_bytes) == NULL);
    ASSERT(slow5_rec_to_mem(read, FORMAT_UNKNOWN, NULL, NULL) == NULL);
    ASSERT(slow5_rec_to_mem(NULL, FORMAT_ASCII, NULL, NULL) == NULL);
    ASSERT(slow5_rec_to_mem(NULL, FORMAT_UNKNOWN, s5p->compress, NULL) == NULL);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_to_mem_change(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    free(read->read_id);
    read->read_id = strdup("testing123");
    char *str;
    ASSERT((str = slow5_rec_to_mem(read, FORMAT_ASCII, NULL, NULL)) != NULL);
    printf("%s", str);
    free(str);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_print_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    ASSERT(slow5_rec_print(read, FORMAT_ASCII, NULL) == 238771);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_print_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);

    ASSERT(slow5_rec_print(NULL, FORMAT_ASCII, NULL) == -1);
    ASSERT(slow5_rec_print(read, FORMAT_UNKNOWN, NULL) == -1);
    ASSERT(slow5_rec_print(NULL, FORMAT_UNKNOWN, NULL) == -1);

    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_print_change(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);

    // TODO add helper to change read id?
    free(read->read_id);
    read->read_id = strdup("lol");

    ASSERT(slow5_rec_print(read, FORMAT_ASCII, NULL) == 238738);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_fprint_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    FILE *fp;
    ASSERT((fp = fopen("test/data/out/unit_test_out_fprint", "w")) != NULL);
    ASSERT(slow5_rec_fprint(fp, read, FORMAT_ASCII, NULL) == 238771);
    slow5_rec_free(read);

    ASSERT(fclose(fp) == 0);
    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_fprint_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    ASSERT(slow5_rec_fprint(NULL, read, FORMAT_ASCII, s5p->compress) == -1);
    ASSERT(slow5_rec_fprint(stdout, NULL, FORMAT_ASCII, NULL) == -1);
    ASSERT(slow5_rec_fprint(NULL, NULL, FORMAT_ASCII, NULL) == -1);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_fprint_change(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = NULL;
    ASSERT(slow5_get_next(&read, s5p) == 0);
    free(read->read_id);
    read->read_id = strdup("lol");
    FILE *fp;
    ASSERT((fp = fopen("test/data/out/unit_test_out_fprint", "a")) != NULL);
    ASSERT(slow5_rec_fprint(fp, read, FORMAT_ASCII, NULL) == 238738);
    slow5_rec_free(read);

    ASSERT(fclose(fp) == 0);
    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_get_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(strcmp(slow5_hdr_get("asic_id", 0, s5p), "3574887596") == 0);
    ASSERT(strcmp(slow5_hdr_get("asic_id_eeprom", 0, s5p), "0") == 0);
    ASSERT(strcmp(slow5_hdr_get("asic_temp", 0, s5p), "29.2145729") == 0);
    ASSERT(strcmp(slow5_hdr_get("auto_update_source", 0, s5p), "https://mirror.oxfordnanoportal.com/software/MinKNOW/") == 0);
    ASSERT(strcmp(slow5_hdr_get("bream_core_version", 0, s5p), "1.1.20.1") == 0);
    ASSERT(strcmp(slow5_hdr_get("usb_config", 0, s5p), "1.0.11_ONT#MinION_fpga_1.0.1#ctrl#Auto") == 0);
    ASSERT(strcmp(slow5_hdr_get("version", 0, s5p), "1.1.20") == 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_get_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_get(NULL, 0, s5p) == NULL);
    ASSERT(slow5_hdr_get("asic_id", -1, s5p) == NULL);
    ASSERT(slow5_hdr_get("asic_id", 1, s5p) == NULL);
    ASSERT(slow5_hdr_get("asic_id", 20, s5p) == NULL);
    ASSERT(slow5_hdr_get("asic_id", UINT32_MAX, s5p) == NULL);
    ASSERT(slow5_hdr_get("asic_id", 0, NULL) == NULL);
    ASSERT(slow5_hdr_get(NULL, 0, NULL) == NULL);
    ASSERT(slow5_hdr_get(NULL, -1, s5p) == NULL);
    ASSERT(slow5_hdr_get("auto_update_source", -1, NULL) == NULL);
    ASSERT(slow5_hdr_get(NULL, -200, NULL) == NULL);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_get_invalid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(strcmp(slow5_hdr_get("asic_id", 0, s5p), "3574887597") != 0);
    ASSERT(strcmp(slow5_hdr_get("asic_id_eeprom", 0, s5p), "100.0") != 0);
    ASSERT(strcmp(slow5_hdr_get("asic_temp", 0, s5p), "292145729") != 0);
    ASSERT(strcmp(slow5_hdr_get("auto_update_source", 0, s5p), "ttps://mirror.oxfordnanoportal.com/software/MinKNOW/") != 0);
    ASSERT(strcmp(slow5_hdr_get("bream_core_version", 0, s5p), "1..20.1") != 0);
    ASSERT(strcmp(slow5_hdr_get("usb_config", 0, s5p), "1.0.11_ONTMinION_fpga_1.0.1#ctrl#Auto") != 0);
    ASSERT(strcmp(slow5_hdr_get("version", 0, s5p), "1.1.2") != 0);

    ASSERT(slow5_hdr_get("file_format", 0, s5p) == NULL);
    ASSERT(slow5_hdr_get("num_read_groups", 0, s5p) == NULL);
    ASSERT(slow5_hdr_get("lol", 0, s5p) == NULL);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_get_set(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    char *asic_id = slow5_hdr_get("asic_id", 0, s5p);
    asic_id[0] = '!';
    ASSERT(strcmp(slow5_hdr_get("asic_id", 0, s5p), "!574887596") == 0);
    asic_id = "new ptr";
    ASSERT(strcmp(slow5_hdr_get("asic_id", 0, s5p), "!574887596") == 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_set_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_set("asic_id", "<new asic id>", 0, s5p) == 0);
    ASSERT(strcmp(slow5_hdr_get("asic_id", 0, s5p), "3574887596") != 0);
    ASSERT(strcmp(slow5_hdr_get("asic_id", 0, s5p), "<new asic id>") == 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_set_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_set("asic_id", "<new asic id>", 0, NULL) == -1);
    ASSERT(slow5_hdr_set("asic_id", "<new asic id>", -1, s5p) == -1);
    ASSERT(slow5_hdr_set("asic_id", NULL, 0, s5p) == -1);
    ASSERT(slow5_hdr_set(NULL, "<new asic id>", 0, s5p) == -1);
    ASSERT(slow5_hdr_set(NULL, NULL, 0, s5p) == -1);
    ASSERT(slow5_hdr_set(NULL, "<new asic id>", -1, s5p) == -1);
    ASSERT(slow5_hdr_set(NULL, "<new asic id>", 0, NULL) == -1);
    ASSERT(slow5_hdr_set(NULL, NULL, 0, NULL) == -1);
    ASSERT(slow5_hdr_set(NULL, NULL, -1, s5p) == -1);
    ASSERT(slow5_hdr_set(NULL, "<new asic id>", -1, NULL) == -1);
    ASSERT(slow5_hdr_set("asic_id", NULL, -1, NULL) == -1);
    ASSERT(slow5_hdr_set(NULL, NULL, -1, NULL) == -1);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_set_invalid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    // Wrong attribute name
    ASSERT(slow5_hdr_set("file_format", "x", 0, s5p) == -1);
    ASSERT(slow5_hdr_set("num_read_groups", "x", 0, s5p) == -1);
    ASSERT(slow5_hdr_set("lol", "x", 0, s5p) == -1);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_set_malloc(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    char *new_version = malloc(10 * sizeof *new_version);
    new_version[0] = '1';
    new_version[1] = '.';
    new_version[2] = '2';
    new_version[3] = '\0';

    ASSERT(slow5_hdr_set("version", new_version, 0, s5p) == 0);
    ASSERT(strcmp(slow5_hdr_get("version", 0, s5p), new_version) == 0);

    free(new_version);
    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_add_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_add("newkey", s5p) == 0);
    ASSERT(slow5_hdr_get("newkey", 0, s5p) == NULL);
    ASSERT(slow5_hdr_set("newkey", "newval", 0, s5p) == 0);
    ASSERT(strcmp(slow5_hdr_get("newkey", 0, s5p), "newval") == 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_add_invalid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_add(NULL, s5p) == -1);
    ASSERT(slow5_hdr_add("k", NULL) == -1);
    ASSERT(slow5_hdr_get("k", 0, s5p) == NULL);
    ASSERT(slow5_hdr_add(NULL, NULL) == -1);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_add_duplicate(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_add("k", s5p) == 0);
    ASSERT(slow5_hdr_get("k", 0, s5p) == NULL);
    ASSERT(slow5_hdr_set("k", "v", 0, s5p) == 0);
    ASSERT(strcmp(slow5_hdr_get("k", 0, s5p), "v") == 0);

    ASSERT(slow5_hdr_add("k", s5p) == -2);
    ASSERT(strcmp(slow5_hdr_get("k", 0, s5p), "v") == 0);
    ASSERT(slow5_hdr_add("k", s5p) == -2);
    ASSERT(slow5_hdr_add("k", s5p) == -2);
    ASSERT(strcmp(slow5_hdr_get("k", 0, s5p), "v") == 0);

    ASSERT(slow5_hdr_add("installation_type", s5p) == -2);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_to_mem_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    void *mem;
    size_t len;
    ASSERT((mem = slow5_hdr_to_mem(s5p, FORMAT_ASCII, &len)) != NULL);
    fwrite(mem, len, 1, stdout);
    free(mem);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_to_mem_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    size_t len;
    ASSERT(slow5_hdr_to_mem(s5p, FORMAT_UNKNOWN, &len) == NULL);
    ASSERT(slow5_hdr_to_mem(NULL, FORMAT_ASCII, &len) == NULL);
    ASSERT(slow5_hdr_to_mem(NULL, FORMAT_UNKNOWN, &len) == NULL);
    ASSERT(slow5_hdr_to_mem(s5p, FORMAT_UNKNOWN, NULL) == NULL);
    ASSERT(slow5_hdr_to_mem(NULL, FORMAT_ASCII, NULL) == NULL);
    ASSERT(slow5_hdr_to_mem(NULL, FORMAT_UNKNOWN, NULL) == NULL);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_to_mem_change_version(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    s5p->header->version.major = 10;
    s5p->header->version.minor = 200;

    char *str;
    ASSERT((str = slow5_hdr_to_mem(s5p, FORMAT_ASCII, NULL)) != NULL);
    printf("%s", str);
    free(str);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_to_mem_change_attr(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_set("filename", "dumbfilename", 0, s5p) == 0);
    ASSERT(slow5_hdr_set("bream_prod_version", "999", 0, s5p) == 0);

    char *str;
    ASSERT((str = slow5_hdr_to_mem(s5p, FORMAT_ASCII, NULL)) != NULL);
    printf("%s", str);
    free(str);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_to_mem_add_empty_attr(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_add("new_attr", s5p) == 0);

    char *str;
    ASSERT((str = slow5_hdr_to_mem(s5p, FORMAT_ASCII, NULL)) != NULL);
    printf("%s", str);
    free(str);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_to_mem_add_attr(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_add("new_attr", s5p) == 0);
    ASSERT(slow5_hdr_set("new_attr", "o328409", 0, s5p) == 0);

    char *str;
    ASSERT((str = slow5_hdr_to_mem(s5p, FORMAT_ASCII, NULL)) != NULL);
    printf("%s", str);
    free(str);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_print_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT((slow5_hdr_print(s5p, FORMAT_ASCII)) > 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_print_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_print(NULL, FORMAT_ASCII) == -1);
    ASSERT(slow5_hdr_print(s5p, FORMAT_UNKNOWN) == -1);
    ASSERT(slow5_hdr_print(NULL, FORMAT_UNKNOWN) == -1);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_print_change_version(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    s5p->header->version.major = 10;
    s5p->header->version.minor = 200;

    ASSERT((slow5_hdr_print(s5p, FORMAT_ASCII)) > 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_print_change_attr(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_set("filename", "dumbfilename", 0, s5p) == 0);
    ASSERT(slow5_hdr_set("bream_prod_version", "999", 0, s5p) == 0);

    ASSERT((slow5_hdr_print(s5p, FORMAT_ASCII)) > 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_print_add_empty_attr(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_add("new_attr", s5p) == 0);

    ASSERT((slow5_hdr_print(s5p, FORMAT_ASCII)) > 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_hdr_print_add_attr(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    ASSERT(slow5_hdr_add("new_attr", s5p) == 0);
    ASSERT(slow5_hdr_set("new_attr", "o328409", 0, s5p) == 0);

    ASSERT((slow5_hdr_print(s5p, FORMAT_ASCII)) > 0);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_idx_init_valid(void) {
    struct slow5_file *s5p = slow5_open("test/data/exp/one_fast5/exp_1_default.slow5", "r");
    ASSERT(s5p != NULL);

    struct slow5_idx *idx = slow5_idx_init(s5p);
    ASSERT(idx != NULL);
    struct slow5_rec_idx read_idx;
    ASSERT(slow5_idx_get(idx, "a649a4ae-c43d-492a-b6a1-a5b8b8076be4", &read_idx) == 0);
    ASSERT(read_idx.offset == 1256);
    ASSERT(read_idx.size == 238771);

    slow5_idx_free(idx);
    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_add_empty(void) {
    remove("test/data/out/exp_1_default_add_empty.slow5.idx");
    struct slow5_file *s5p = slow5_open("test/data/out/exp_1_default_add_empty.slow5", "r+");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = slow5_rec_init();
    read->read_id = strdup("");
    ASSERT(slow5_rec_add(read, s5p) == 0);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_add_null(void) {
    struct slow5_file *s5p = slow5_open("test/data/out/exp_1_default_add_empty.slow5", "r+");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = slow5_rec_init();
    read->read_id = strdup("");
    ASSERT(slow5_rec_add(read, NULL) == -1);
    ASSERT(slow5_rec_add(NULL, s5p) == -1);
    ASSERT(slow5_rec_add(NULL, NULL) == -1);
    free(read->read_id);
    read->read_id = NULL;
    ASSERT(slow5_rec_add(read, s5p) == -1);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_add_valid(void) {
    remove("test/data/out/exp_1_default_add_valid.slow5.idx");
    struct slow5_file *s5p = slow5_open("test/data/out/exp_1_default_add_valid.slow5", "r+");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = slow5_rec_init();
    ASSERT(slow5_get_next(&read, s5p) == 0);

    read->read_id[strlen(read->read_id) - 1] = '\0';

    ASSERT(slow5_rec_add(read, s5p) == 0);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}

int slow5_rec_add_duplicate(void) {
    remove("test/data/out/exp_1_default_add_duplicate.slow5.idx");
    struct slow5_file *s5p = slow5_open("test/data/out/exp_1_default_add_duplicate.slow5", "r+");
    ASSERT(s5p != NULL);

    struct slow5_rec *read = slow5_rec_init();
    ASSERT(slow5_get_next(&read, s5p) == 0);
    ASSERT(slow5_rec_add(read, s5p) == -3);
    slow5_rec_free(read);

    ASSERT(slow5_close(s5p) == 0);
    return EXIT_SUCCESS;
}


int main(void) {

    struct command tests[] = {
        // API
        CMD(slow5_open_valid)
        CMD(slow5_open_null)
        CMD(slow5_open_invalid)

        CMD(slow5_open_with_valid)
        CMD(slow5_open_with_null)
        CMD(slow5_open_with_invalid)

        CMD(slow5_get_valid)
        CMD(slow5_get_null)
        CMD(slow5_get_invalid)
        CMD(slow5_get_many_same)
        CMD(slow5_get_many_different)

        CMD(slow5_get_next_valid)
        CMD(slow5_get_next_null)
        CMD(slow5_get_next_empty)
        CMD(slow5_get_next_invalid)

        CMD(slow5_rec_to_mem_valid)
        CMD(slow5_rec_to_mem_null)
        CMD(slow5_rec_to_mem_change)

        CMD(slow5_rec_print_valid)
        CMD(slow5_rec_print_null)
        CMD(slow5_rec_print_change)

        CMD(slow5_rec_fprint_valid)
        CMD(slow5_rec_fprint_null)
        CMD(slow5_rec_fprint_change)

        CMD(slow5_hdr_get_valid)
        CMD(slow5_hdr_get_null)
        CMD(slow5_hdr_get_invalid)
        CMD(slow5_hdr_get_set)

        CMD(slow5_hdr_set_valid)
        CMD(slow5_hdr_set_null)
        CMD(slow5_hdr_set_invalid)
        CMD(slow5_hdr_set_malloc)

        CMD(slow5_hdr_add_valid)
        CMD(slow5_hdr_add_invalid)
        CMD(slow5_hdr_add_duplicate)

        CMD(slow5_hdr_to_mem_valid)
        CMD(slow5_hdr_to_mem_null)
        CMD(slow5_hdr_to_mem_change_version)
        CMD(slow5_hdr_to_mem_change_attr)
        CMD(slow5_hdr_to_mem_add_empty_attr)
        CMD(slow5_hdr_to_mem_add_attr)

        CMD(slow5_hdr_print_valid)
        CMD(slow5_hdr_print_null)
        CMD(slow5_hdr_print_change_version)
        CMD(slow5_hdr_print_change_attr)
        CMD(slow5_hdr_print_add_empty_attr)
        CMD(slow5_hdr_print_add_attr)

        CMD(slow5_idx_init_valid)

        CMD(slow5_rec_add_empty)
        CMD(slow5_rec_add_null)
        CMD(slow5_rec_add_valid)
        CMD(slow5_rec_add_duplicate)
    };

    return RUN_TESTS(tests);
}