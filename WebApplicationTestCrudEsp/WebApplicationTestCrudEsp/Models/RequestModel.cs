using Newtonsoft.Json;

namespace WebApplicationTestCrudEsp.Models
{
    public class RequestModel
    {
        [JsonProperty("id")]
        public int Id { get; set; }
        [JsonProperty("name")]
        public string Name { get; set; }
        [JsonProperty("description")]
        public string Description { get; set; }
        [JsonProperty("data")]
        public double[] Data { get; set; }
    }
}
