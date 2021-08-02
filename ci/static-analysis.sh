#!/bin/bash
# SPDX-License-Identifier: GPL-2.0
#
# Copyright (c) 2021 Canonical Ltd.
# Author: Krzysztof Kozlowski <krzysztof.kozlowski@canonical.com>
#                             <krzk@kernel.org>
# Derived from:
# https://github.com/Tlazypanda/cpp-clang-check/blob/master/checkall.sh
#

set -eEx

if [[ -z "$GITHUB_TOKEN" ]]; then
	echo "The GITHUB_TOKEN is required."
	exit 1
fi

echo $GITHUB_EVENT_PATH
cat $GITHUB_EVENT_PATH

FILES_LINK=`jq -r '.pull_request._links.self.href' "$GITHUB_EVENT_PATH"`/files
echo "Files = $FILES_LINK"

curl $FILES_LINK > files.json
FILES_URLS_STRING=`jq -r '.[].raw_url' files.json`

readarray -t URLS <<<"$FILES_URLS_STRING"

echo "File names: $URLS"

mkdir files
cd files
for i in "${URLS[@]}"
do
   echo "Downloading $i"
   curl -LOk --remote-name $i
done

echo "Files downloaded!"
echo "Performing checkup:"
clang-tidy --version
clang-tidy *.cpp -checks=boost-*,bugprone-*,performance-*,readability-*,portability-*,modernize-*,clang-analyzer-cplusplus-*,clang-analyzer-*,cppcoreguidelines-* > clang-tidy-report.txt

cppcheck -iclang-format-report.txt -iclang-tidy-report.txt --enable=all --std=c++11 --language=c++ --output-file=cppcheck-report.txt *

flawfinder --columns --context --singleline . > flawfinder-report.txt

PAYLOAD_TIDY=`cat clang-tidy-report.txt`
PAYLOAD_CPPCHECK=`cat cppcheck-report.txt`
PAYLOAD_FLAWFINDER=`cat flawfinder-report.txt`
COMMENTS_URL=$(cat $GITHUB_EVENT_PATH | jq -r .pull_request.comments_url)

echo $COMMENTS_URL
echo "Clang-tidy errors:"
echo $PAYLOAD_TIDY
echo "Cppcheck errors:"
echo $PAYLOAD_CPPCHECK
echo "Flawfinder errors:"
echo $PAYLOAD_FLAWFINDER

OUTPUT=$'**CLANG-TIDY WARNINGS**:\n'
OUTPUT+=$'\n```\n'
OUTPUT+="$PAYLOAD_TIDY"
OUTPUT+=$'\n```\n'

OUTPUT+=$'\n**CPPCHECK WARNINGS**:\n'
OUTPUT+=$'\n```\n'
OUTPUT+="$PAYLOAD_CPPCHECK"
OUTPUT+=$'\n```\n'

OUTPUT+=$'\n**FLAWFINDER WARNINGS**:\n'
OUTPUT+=$'\n```\n'
OUTPUT+="$PAYLOAD_FLAWFINDER"
OUTPUT+=$'\n```\n'

PAYLOAD=$(echo '{}' | jq --arg body "$OUTPUT" '.body = $body')

curl -s -S -H "Authorization: token $GITHUB_TOKEN" --header "Content-Type: application/vnd.github.VERSION.text+json" --data "$PAYLOAD" "$COMMENTS_URL"
