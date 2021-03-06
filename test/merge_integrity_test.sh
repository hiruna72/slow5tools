#!/bin/bash
# merge four single group slow5s and diff with expected slow5 to check if slow5tools merge is working as expected

# WARNING: this file should be stored inside test directory
# WARNING: the executable should be found at ../
# WARNING: four slow5s should be found at ./data/exp/merge/slow5s
# WARNING: expected slow5 should be found at ./data/exp/merge

Usage="merge_integrity.sh"

# Relative path to "slow5/tests/"
REL_PATH="$(dirname $0)/" 

NC='\033[0m' # No Color
RED='\033[0;31m'
GREEN='\033[0;32m'

if [ "$1" = 'mem' ]; then
    SLOW5_EXEC="valgrind --leak-check=full --error-exitcode=1 $REL_PATH/../slow5tools"
else
    SLOW5_EXEC=$REL_PATH/../slow5tools
fi

OUTPUT_DIR="$REL_PATH/data/out/merge"
test -d  $OUTPUT_DIR
rm -r $OUTPUT_DIR
mkdir $OUTPUT_DIR

INPUT_FILE=$REL_PATH/data/exp/merge/slow5s
INPUT_FILES="$INPUT_FILE/rg0.slow5 $INPUT_FILE/rg1.slow5 $INPUT_FILE/rg2.slow5 $INPUT_FILE/rg3.slow5"

echo "-------------------tesetcase 0: slow5tools version-------------------"
if ! $SLOW5_EXEC --version; then
    echo -e "${RED}tesetcase 0: slow5tools version failed${NC}"
    exit 1
fi

echo
echo "-------------------tesetcase 1: lossless merging-------------------"
if ! $SLOW5_EXEC merge $INPUT_FILES -o $OUTPUT_DIR/merged_output.slow5 --to slow5; then
    echo -e "${RED}tesetcase 1: lossless merging failed${NC}"
    exit 1
fi
echo "comparing merged_output and merged_expected"
sort $REL_PATH/data/exp/merge/merged_expected.slow5 > $OUTPUT_DIR/merged_expected_sorted.slow5
sort $OUTPUT_DIR/merged_output.slow5 > $OUTPUT_DIR/merged_output_sorted.slow5
rm $OUTPUT_DIR/merged_output.slow5
cmp -s $OUTPUT_DIR/merged_expected_sorted.slow5 $OUTPUT_DIR/merged_output_sorted.slow5
if [ $? -eq 0 ]; then
    echo -e "${GREEN}SUCCESS: merged files are consistent!${NC}"
elif [ $? -eq 1 ]; then
    echo -e "${RED}FAILURE: merged files are not consistent${NC}"
    exit 1
else
    echo -e "${RED}ERROR: diff failed for some weird reason${NC}"
    exit 1
fi


echo
echo "-------------------tetcase 2: lossy merging-------------------"
if ! $SLOW5_EXEC merge -l false $INPUT_FILES -o $OUTPUT_DIR/lossy_merged_output.slow5 --to slow5; then
    echo -e "${RED}tetcase 2: lossy merging failed${NC}"
    exit 1
fi
echo "comparing lossy_merged_output and lossy_merged_expected"
sort $REL_PATH/data/exp/merge/lossy_merged_expected.slow5 > $OUTPUT_DIR/lossy_merged_expected_sorted.slow5
sort $OUTPUT_DIR/lossy_merged_output.slow5 > $OUTPUT_DIR/lossy_merged_output_sorted.slow5
rm $OUTPUT_DIR/lossy_merged_output.slow5
cmp -s $OUTPUT_DIR/lossy_merged_expected_sorted.slow5 $OUTPUT_DIR/lossy_merged_output_sorted.slow5
if [ $? -eq 0 ]; then
    echo -e "${GREEN}SUCCESS: lossy merged files are consistent!${NC}"
elif [ $? -eq 1 ]; then
    echo -e "${RED}FAILURE: lossy merged files are not consistent${NC}"
    exit 1
else
    echo -e "${RED}ERROR: diff failed for some weird reason${NC}"
    exit 1
fi

exit
