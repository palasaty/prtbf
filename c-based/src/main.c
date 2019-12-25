#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../proto/ComputationalLifeProtocol.pb-c.h"
#include <protobuf2json.h>

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

	march.duration_s = 0.01;
	char *json = NULL;
	protobuf2json_string(
		&march.base, 
		(JSON_INDENT(2) | JSON_PRESERVE_ORDER), 
		&json, NULL, 0
	);
	printf("march json: %s\n", json);

	free(json);
}

int main()
{
	printf("Start protobuf program\n");
	March_c();
	return 0;
}
