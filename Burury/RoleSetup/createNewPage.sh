#!/bin/bash

dir=$1
name=$2

tmpl_cpp="page_template_cpp"
tmpl_hpp="page_template_hpp"
tmpl_ui="page_template_ui"

cat $tmpl_cpp | sed "s/RENAME_ME/$name/g" | sed "s/DIR_NAME/$dir/g" > $dir/src/${name}.cpp
cat $tmpl_ui  | sed "s/RENAME_ME/$name/g" > $dir/Ui/${name}.ui
cat $tmpl_hpp | sed "s/RENAME_ME/$name/g" > $dir/include/RoleSetup/$dir/${name}.hpp
