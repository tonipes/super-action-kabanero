#!/usr/bin/python3

# http://addamhardy.com/2013/06/05/good-commit-messages-and-enforcing-them-with-git-hooks.html
import re, sys

# check that there is :xxx: (emoji) and space in the begining of the line
# xxx is between 3 and 20 letters
FIRST_LINE_REGEX = r"^:[a-z]{3,20}: .*$"

message_file = sys.argv[1]

def check_first_line_format(line):
    match = re.search(FIRST_LINE_REGEX, line)
    if match:
        return False
    else:
        return "Error: First line does not match required format."

def check_format_rules(lineno, line):
    real_lineno = lineno + 1
    if lineno == 0:
        if len(line) > 50:
            return "Error %d: First line should be less than 50 characters " \
                    "in length." % (real_lineno,)
        return check_first_line_format(line)
    if lineno == 1:
        if line:
            return "Error %d: Second line should be empty." % (real_lineno,)
    if not line.startswith('#'):
        if len(line) > 72:
            return "Error %d: No line should be over 72 characters long." % (
                    real_lineno,)
    return False

print("Checking commit message")

with open(message_file, "r") as f:
    commit_msg = []
    errors = []

    for i, line in enumerate(f):
        stripped_line = line.strip()
        commit_msg.append(line)
        e = check_format_rules(i, stripped_line)
        if e:
            errors.append(e)
    if errors:
        print("Commit aborted. Errors in commit messages")
        for e in errors:
            print(e)
        sys.exit(1)
    else:
        sys.exit(0)
