#!/bin/sh
set -x
dialog --checklist 'Choose the desired patches' 0 0 0 \
	fn-swap		'Swap Fn and Ctrl keys'				off \
	lcd-brightness	'Allow lowering brightness to 1%'		on \
	2> selected
for p in $(cat selected); do
	fn="patches/$p.rapatch"
	r2 -w -q -P "$fn" ec.bin || true	# see https://github.com/radare/radare2/issues/15002
done
