.PHONY: all get_input_files test update_input_check update_output_check

HISUI=../../release/hisui

all: test

get_input_files:
	[ -f input/caminandes3.webm ] || curl -o input/caminandes3.webm https://ia801606.us.archive.org/24/items/CaminandesLlamigos/Caminandes_%20Llamigos-1080p.webm # CC BY 3.0 https://creativecommons.org/licenses/by/3.0/ (c) copyright 2016, Blender Foundation / http://www.caminandes.com/
	[ -f input/lilac_blossom_bloom_spring.jpg ] || curl -o input/lilac_blossom_bloom_spring.jpg https://free-images.com/tn/c1df/lilac_blossom_bloom_spring.jpg # CC0
	[ -f input/Big_Buck_Bunny_360_10s_1MB.webm ] || curl -o input/Big_Buck_Bunny_360_10s_1MB.webm https://test-videos.co.uk/vids/bigbuckbunny/webm/vp9/360/Big_Buck_Bunny_360_10s_1MB.webm # CC BY 3.0 https://creativecommons.org/licenses/by/3.0/ (c) copyright 2008, Blender Foundation / www.bigbuckbunny.org
	sha224sum -c input/check

test: get_input_files
	rm output/*.webm*
	for m in metadata/*.json; do \
		base=$$(basename $${m} .json); \
		echo $${base}; \
		vp9_file=output/$${base}.vp9.webm; \
		vp8_file=output/$${base}.vp8.webm; \
		${HISUI} -f $${m} --out-webm-file $${vp9_file} --show-progress-bar false --log-level error --out-video-codec VP9; \
		${HISUI} -f $${m} --out-webm-file $${vp8_file} --show-progress-bar false --log-level error --out-video-codec VP8; \
		tail --bytes=+$$(bash ./get_cluster_start_position.bash $${vp9_file}) $${vp9_file} | head --bytes=$$(bash ./get_cluster_size.bash $${vp9_file}) > $${vp9_file}.cluster; \
		tail --bytes=+$$(bash ./get_cluster_start_position.bash $${vp8_file}) $${vp8_file} | head --bytes=$$(bash ./get_cluster_size.bash $${vp8_file}) > $${vp8_file}.cluster; \
	done
	sha224sum -c output/check

update_input_check:
	sha224sum input/*.webm input/*.jpg > input/check

update_output_check:
	sha224sum output/*.cluster > output/check
