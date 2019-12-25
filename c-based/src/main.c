#include <stdio.h>
#include <stdlib.h>
#include "../proto/ComputationalLifeProtocol.pb-c.h"

/*void March()
{
  std::string json;
  // Create the main interchange objet
  ModelInterchangeData* march_exchange = new ModelInterchangeData();
  MarchData* march = new MarchData();
  march->set_duration_s(0.001);
  march_exchange->set_allocated_march(march);
  // Write this object out in JSON
  google::protobuf::util::JsonPrintOptions opts;
  opts.preserve_proto_field_names = true;
  bool ret = google::protobuf::util::MessageToJsonString(*march_exchange, &json, opts).ok();
  std::cout << json << "\n\n\n";
  std::ofstream myfile;
  myfile.open("4_March.json");
  myfile << json;
  myfile.close();
}*/

void March_c()
{
	MarchData march = MARCH_DATA__INIT;
	void *buf;
	unsigned len;

	march.duration_s = 0.001;
	len = march_data__get_packed_size(&march);

	buf = malloc(len);
	march_data__pack(&march, buf);
	printf("march json: %s", buf);

	free(buf);
}

int main()
{
	printf("Start protobuf program\n");
	March_c();
	return 0;
}
