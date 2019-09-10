#!/bin/sh
dialog --checklist 'Choose the desired patches' 0 0 0 fn-swap 'Swap Fn and Ctrl keys' off 2> selected
for p in $(cat selected); do
	fn="patches/$p.rapatch"
	r2 -w -q -P "$fn" ec.bin
done
