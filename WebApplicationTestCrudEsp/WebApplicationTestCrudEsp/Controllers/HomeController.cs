using System;
using Newtonsoft.Json;
using Microsoft.AspNetCore.Mvc;
using WebApplicationTestCrudEsp.Models;

namespace WebApplicationTestCrudEsp.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class HomeController : ControllerBase
    {
        /// <summary>
        /// Create
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        [AcceptVerbs("POST"), Route("test")]
        public ResponseModel PostData(RequestModel request)
        {
            throw new NotImplementedException();
        }
        /// <summary>
        /// Read
        /// </summary>
        /// <param name="Id"></param>
        /// <returns></returns>
        [AcceptVerbs("GET"), Route("test/{Id}")]
        public ResponseModel GetParameters(int Id)
        {
            ConsoleColor tmp = Console.ForegroundColor;
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.WriteLine("GET test/" + Id);
            ResponseModel response;
            if (Id == 1)
            {
                response = new ResponseModel
                {
                    Id = 1,
                    Name = "Dev-1",
                    Command = "continue",
                    Parameters = new string[] { "Test-1", "Test-2" }
                };
            }
            else
            {
                response = new ResponseModel
                {
                    Id = Id,
                    Name = null,
                    Command = "fail",
                    Parameters = new string[] { "not found" }
                };
            }
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("Response:");
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine(JsonConvert.SerializeObject(response, Formatting.Indented));
            Console.ForegroundColor = tmp;
            return response;
        }

        /// <summary>
        /// Update
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        [AcceptVerbs("PUT"), Route("test")]
        public ResponseModel PutData(RequestModel request)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Delete
        /// </summary>
        /// <param name="Id"></param>
        /// <returns></returns>
        [AcceptVerbs("DELETE"), Route("test/{Id}")]
        public ResponseModel Delete(int Id)
        {
            throw new NotImplementedException();
        }
    }
}
