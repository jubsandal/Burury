#!/bin/bash

locale="en"

declare -A localization
case $locale in
        "ru")
                locale["Table of content"]="Содержание"
                ;;
        "en")
                locale["Table of content"]="Table of content"
                ;;
esac

function indexGen() {
        out="tmp.indexed.md"
        index="tmp.index.md"

        cp "$1" "$out"

        tab='\t'
        i=(-1 1)
        prevLen=0
        while read -r line
        do
                hash="$(md5sum <<< "$line" | cut -d ' ' -f 1)"
                printf "<a id=\"%s\"/>\n\n%s\n\n" "$hash" "$line" > tmp
                sed "/$line/ {
                        x
                        r tmp
                }" "$out" > "${out}.tmp"
                mv "${out}.tmp" "$out"
                hdrLen=$(awk -F'#' '{print NF-1}' <<< "$line")
                hdrTxt="${line//#/}"
                (( hdrLen > 1 )) && for (( j=1; j<hdrLen; j++ )); do echo -en "$tab"; done
                (( prevLen < hdrLen )) && i[$hdrLen]=1
                printf "%d. [%s ](#%s)\n" ${i[$hdrLen]} "$hdrTxt" "$hash"
                prevLen=$hdrLen
                let i[$hdrLen]++
        done <<< "$(grep --color=no -E "^#+ " "$1")" > "$index"

        cat "$out" > "$out".tmp
        printf "# %s\n" "${localization["Table of content"]}" > "$out"
        cat "$index" >> "$out"
        cat "$out".tmp >> "$out"

        rm "$index"
        rm tmp
        rm "$out".tmp

        mv "$out" "$2"
}

function umlGen() {
        plantuml -tsvg "$1"/*
        mv "$1"/*.svg "$2"/
}

function htmlGen() {
        pandoc -f markdown -s "$1" -o "$2"
}

function pdfGen() {
        pandoc -s -t latex \
                -V mainfont="$1" \
                -f markdown \
                "$2" -o "$3"
}

function docxGen() {
        pandoc -s -o "$2" "$1"
}

pdfFont="Deja Vu:style=Regular:size=20"

input="_index.md"
umlDir="uml"
logoDir="logo"

mdOut="../docs/index.md"
pdfOut="../docs/index.pdf"
docxOut="../docs/index.docx"
imgDirOut="../docs/img"

[[ -d "$imgDirOut" ]] || mkdir -p "$imgDirOut"
umlGen "$umlDir" "$imgDirOut"
cp "$logoDir"/* "$imgDirOut"/
indexGen "$input" "$mdOut"
pdfGen "$pdfFont" "$mdOut" "$pdfOut"
docxGen "$mdOut" "$docxOut"
