using Newtonsoft.Json;

namespace WebApplicationTestCrudEsp.Models
{
    public class ResponseModel
    {
        [JsonProperty("id")]
        public int Id { get; set; }
        [JsonProperty("name")]
        public string Name { get; set; }
        [JsonProperty("command")]
        public string Command { get; set; }
        [JsonProperty("parameters")]
        public string[] Parameters { get; set; }
    }
}
