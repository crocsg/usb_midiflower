
#include <string.h>
#include <deque>

#include "lwip/init.h"
#include "lwip/timeouts.h"
#include "lwip/ethip6.h"
#include "lwip/apps/httpd.h"
#include "utils.h"
#include "flower_sensor.h"

#define MAX_SSI_TAG_LEN	LWIP_HTTPD_MAX_TAG_NAME_LEN
#define CRLF "\r\n"


typedef struct _ssi_app_tags
{
	uint8_t	id;
	char	tag[MAX_SSI_TAG_LEN + 1];
} ssi_app_tags;

#ifdef __cplusplus
extern "C" {
#endif
static const char* ssi_tags[] = {
  "flwdata",
  "curip"
};

static const tCGI cgi_handlers[] = {
  {
    "/config/data/bidon/data.json",
    NULL
  },
  
};

u16_t ssi_app_ssi_handler(
                             int iIndex,

                             char *pcInsert, int iInsertLen
#if LWIP_HTTPD_SSI_MULTIPART
                             , u16_t current_tag_part, u16_t *next_tag_part
#endif /* LWIP_HTTPD_SSI_MULTIPART */
                             )
{
#if LWIP_HTTPD_SSI_MULTIPART
	(void) current_tag_part;
	(void) next_tag_part;
#endif
	LWIP_DEBUGF(HTTP_SSI_DEBUG, ("SSI HANDLER\n"));
	if (iIndex < ARRAYLEN(ssi_tags))
	{
		LWIP_DEBUGF(HTTP_SSI_DEBUG, ("tag_name :%s\n", ssi_tags[iIndex]));
    LWIP_DEBUGF(HTTP_SSI_DEBUG, ("info size :%d\n", iInsertLen));
	}
	LWIP_DEBUGF(HTTP_SSI_DEBUG, ("request :%d\n", iIndex));

	uint16_t printed = 0;
	uint16_t pdata = 0;
	uint16_t n = 0;
  uint32_t starttime = 0;

  switch (iIndex)
	{
	case 0: // flowerdata
		{
    std::deque<mes_data>& data =  flower_sensor_get_history ();

    if (data.size () > 0)
      starttime = data[0].time;
    for (auto it = data.begin(); it != data.end() && iInsertLen > 32; ++it)
    {
      pdata = snprintf(&pcInsert[printed], iInsertLen, "%s{\"id\":%lu,\"value\":%lu}", 
             n == 0 ? "":",",
             it->time - starttime,
             it->data
             
            );
      n++;      
      printed += pdata;
      iInsertLen -= pdata;            
    }
    
    }
		break;
	case 1: // flowerdata
		
    printed = snprintf(pcInsert, iInsertLen, " "); 
           
		break;
	

	}

	return printed;
}

#ifdef __cplusplus
}
#endif



const char ** http_get_html_app_tags(void)
{

	return (const char**) ssi_tags;
}

uint16_t http_get_html_app_tags_nbr(void)
{
	return (sizeof(ssi_tags)/ sizeof(ssi_tags[0]));
}

//ypedef u16_t(* tSSIHandler) (const char *ssi_tag_name, char *pcInsert, int iInsertLen)



void http_server_init (void)
{
    httpd_init();
    http_set_ssi_handler(ssi_app_ssi_handler,  (const char**) ssi_tags, ARRAYLEN(ssi_tags));
    http_set_cgi_handlers(cgi_handlers, ARRAYLEN(cgi_handlers));
    
}