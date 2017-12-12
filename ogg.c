#include <ogg/ogg.h>

int main(void)
{
	ogg_sync_state oy;
	ogg_page       og;
	ogg_packet     op;
	ogg_stream_state os;

	ogg_sync_init(&oy);

	while (1) {
		char *data;
		int i, nb_read;

		data = ogg_sync_buffer(&oy, 200);
		nb_read = fread(data, sizeof(char), 200, fin);
		gg_sync_wrote(&oy, nb_read);

		while (ogg_sync_pageout(&oy, &og)==1) {
			if (stream_init == 0) { 
				ogg_stream_init(&os, ogg_page_serialno(&og));
				stream_init = 1; 
			}

		}
	}

	return 0;
}
