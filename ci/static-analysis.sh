#!/bin/bash
# SPDX-License-Identifier: GPL-2.0
#
# Copyright (c) 2021 Canonical Ltd.
# Author: Krzysztof Kozlowski <krzysztof.kozlowski@canonical.com>
#                             <krzk@kernel.org>
#

set -ex

OUTPUT_CPPCHECK=out/report-cppcheck.txt
OUTPUT_FLAWFINDER=out/report-flawfinder.txt

mkdir -p out

cppcheck --output-file=$OUTPUT_CPPCHECK ./

flawfinder --columns --context --singleline . > $OUTPUT_FLAWFINDER

REPORT_CPPCHECK=`cat $OUTPUT_CPPCHECK`
REPORT_FLAWFINDER=`cat $OUTPUT_FLAWFINDER`
COMMENTS_URL=$(cat $GITHUB_EVENT_PATH | jq -r .pull_request.comments_url)

REPORT=""
REPORT+="################################"
REPORT+="CPPCHECK WARNINGS\n"
REPORT+="$REPORT_CPPCHECK"
REPORT+="\n\n"
REPORT+="################################"
REPORT+="FLAWFINDER WARNINGS\n"
REPORT+="$REPORT_FLAWFINDER"
REPORT+="\n\n"

DATA=$(echo '{}' | jq --arg body "$REPORT" '.body = $body')

curl -s -S -H "Authorization: token $GITHUB_TOKEN" --header "Content-Type: application/vnd.github.VERSION.text+json" --data "$DATA" "$COMMENTS_URL"
