
#include <string.h>
#include <deque>

#include "lwip/init.h"
#include "lwip/timeouts.h"
#include "lwip/ethip6.h"
#include "lwip/apps/httpd.h"
#include "lwip/def.h"

#include "utils.h"
#include "flower_sensor.h"
#include "flower_music.h"

#define MAX_SSI_TAG_LEN	LWIP_HTTPD_MAX_TAG_NAME_LEN
#define CRLF "\r\n"


extern CMidiFlowerSequencer sequencer;

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
  "curip",
  "jscales",
  "curscale",
  "jnotes",
  "curnote",
  "jbpms",
  "curbpm",
  "channels"
};

static const char *cgi_setdata(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
static const char *cgi_setchan(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

static const tCGI cgi_handlers[] = {
  {
    "/config/data/bidon/data.json",
    NULL
  },
  {
    "/config/setdata/data",
    cgi_setdata
  },
  {
    "/config/setdata/chan",
    cgi_setchan
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
	LWIP_DEBUGF(HTTP_SSI_DEBUG, ("SSI HANDLER\n"));
	if (iIndex < ARRAYLEN(ssi_tags))
	{
		LWIP_DEBUGF(HTTP_SSI_DEBUG, ("tag_name :%s\n", ssi_tags[iIndex]));
    LWIP_DEBUGF(HTTP_SSI_DEBUG, ("info size :%d\n", iInsertLen));
	}
	LWIP_DEBUGF(HTTP_SSI_DEBUG, ("request :%d\n", iIndex));

	uint16_t printed = 0;
	uint16_t pdata = 0;
	uint16_t n = current_tag_part;
  uint32_t starttime = 0;
  uint32_t startdata = 0;
  uint16_t i;

  switch (iIndex)
	{
	case 0: // flwdata
		{
    std::deque<mes_data>& data =  flower_sensor_get_history ();

    if (data.size () > 0)
    {
      starttime = data[0].time;
      startdata = data[0].data;
    }
    uint16_t pos = current_tag_part * 4;  
    for (i = pos; i < pos + 4 && iInsertLen > 16 && i < data.size () ; i++)
    {
      pdata = snprintf(&pcInsert[printed], iInsertLen, "%s{\"id\":%lu,\"value\":%ld}", 
             n == 0 ? "":",",
             i,
             (int) ((int) data[i].data - (int) startdata)
             
            );
      n++;      
      printed += pdata;
      iInsertLen -= pdata;            
    }
    if (i < data.size ())
      *next_tag_part = current_tag_part + 1;

    }
		break;
	case 1: // curip
		
    printed = snprintf(pcInsert, iInsertLen, " "); 
           
		break;
  case 2: // jscales
  {
    const char** scales = flower_music_get_scale_names ();
    uint8_t nbscale = flower_music_get_scale_names_nbr ();
    printed = 0;
    for (uint8_t i = 0; i < nbscale && iInsertLen > 16 ; i++)
    {
      pdata = snprintf(&pcInsert[printed], iInsertLen, "\"%s\"%s", scales[i], i == nbscale - 1 ? "" : ","); 
      printed += pdata;
      iInsertLen -= pdata;
    }
  }
    break;
  case 3: // curscale
  {
    const char** scales = flower_music_get_scale_names ();
    uint8_t nbscale = flower_music_get_scale_names_nbr ();
    uint8_t curscale = flower_music_get_scale();
    printed = 0;
    if (curscale < nbscale)
    {
      printed += snprintf(&pcInsert[printed], iInsertLen, "%s", scales[curscale]); 
      
    }
  }
    break;
  
  case 4: // "jnotes",
  {
    const char** notes = flower_music_get_note_names ();
    uint8_t nbnote = flower_music_get_note_names_nbr ();
    printed = 0;
    for (uint8_t i = 0; i < nbnote && iInsertLen > 16 ; i++)
    {
      pdata = snprintf(&pcInsert[printed], iInsertLen, "\"%s\"%s", notes[i], i == nbnote - 1 ? "" : ","); 
      printed += pdata;
      iInsertLen -= pdata;
    }
  }
    break;
  
  case 5: //"curnote"  
  {
    const char** notes = flower_music_get_note_names ();
    uint8_t nbnote = flower_music_get_note_names_nbr ();
    uint8_t curnote = flower_music_get_current_root();
    printed = 0;
    if (curnote < nbnote)
    {
      printed += snprintf(&pcInsert[printed], iInsertLen, "%s", notes[curnote]); 
      
    }
  }
    break;

  case 6: // "jbpms",
  {
    uint16_t* bpms = flower_music_get_bpm_values ();
    uint8_t nbbpm = flower_music_get_bpm_values_nbr ();
    printed = 0;
    for (uint8_t i = 0; i < nbbpm && iInsertLen > 16 ; i++)
    {
      pdata = snprintf(&pcInsert[printed], iInsertLen, "%u%s", bpms[i], i == nbbpm - 1 ? "" : ","); 
      printed += pdata;
      iInsertLen -= pdata;
    }
  }
    break;  

  case 7: // "curbpm",
  {
    printed = 0;
    printed += snprintf(&pcInsert[printed], iInsertLen, "%u",(uint16_t)  flower_music_get_basebpm()); 
  }
    break;    
	
  case 8: // channels,
  {
    
    uint8_t nbchannel = sequencer.get_nbtracks ();
    printed = 0;
    for (uint8_t i = 0; i < nbchannel && iInsertLen > 8 ; i++)
    {
      pdata = snprintf(&pcInsert[printed], iInsertLen, "{\"mul\":%u,\"size\":%u,\"ratio\":%u,\"vol\":%u}%s", 
        sequencer.get_track_mulbpm(i), 
        sequencer.get_track_size(i), 
        sequencer.get_track_ratio(i), 
        sequencer.get_track_relative_vol(i), 
        i == nbchannel - 1 ? "" : ","); 
      printed += pdata;
      iInsertLen -= pdata;
    }
  }
    break;  
  }
  
  

	return printed;
}

static const char *cgi_setdata(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint16_t paramval =0;
   LWIP_ASSERT("check index", iIndex < LWIP_ARRAYSIZE(cgi_handlers));
  if (iNumParams > 0)
  {
    paramval = atoi (pcValue[0]);
    if (strncmp (pcParam[0], "scale", LWIP_HTTPD_MAX_TAG_NAME_LEN ) == 0)
    {
      LWIP_DEBUGF(HTTP_CGI_DEBUG, ("scale config :%s %d\n", pcValue[0], paramval));
      flower_music_set_scale (paramval);
    }
    else if (strncmp (pcParam[0], "root", LWIP_HTTPD_MAX_TAG_NAME_LEN ) == 0)
    {
      LWIP_DEBUGF(HTTP_CGI_DEBUG, ("root config :%s %d\n", pcValue[0], paramval));
      flower_music_set_root (paramval);
    }
    else if (strncmp (pcParam[0], "bpm", LWIP_HTTPD_MAX_TAG_NAME_LEN ) == 0)
    {
      LWIP_DEBUGF(HTTP_CGI_DEBUG, ("bpm config :%s %d\n", pcValue[0], paramval));
      flower_music_set_basebpm (paramval);
    }
  }
  return ("/config/data/empty.json");
}

static const char *cgi_setchan(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint16_t paramval =0;
  uint16_t channel = 0;
   LWIP_ASSERT("check index", iIndex < LWIP_ARRAYSIZE(cgi_handlers));
  if (iNumParams > 1)
  {
    channel = atoi (pcValue[0]);
    paramval = atoi (pcValue[1]);
    if (strncmp (pcParam[1], "mul", LWIP_HTTPD_MAX_TAG_NAME_LEN ) == 0)
    {
      LWIP_DEBUGF(HTTP_CGI_DEBUG, ("mul config :%d %s %d\n", channel, pcValue[0], paramval));
      sequencer.set_track_mulbpm (channel, paramval);
    }
    else if (strncmp (pcParam[1], "size", LWIP_HTTPD_MAX_TAG_NAME_LEN ) == 0)
    {
      LWIP_DEBUGF(HTTP_CGI_DEBUG, ("size config :%d %s %d\n", channel, pcValue[0], paramval));
      sequencer.set_track_size (channel, paramval);
    }
    else if (strncmp (pcParam[1], "ratio", LWIP_HTTPD_MAX_TAG_NAME_LEN ) == 0)
    {
      LWIP_DEBUGF(HTTP_CGI_DEBUG, ("ratio config :%d %s %d\n", channel, pcValue[0], paramval));
      sequencer.set_track_ratio (channel, paramval);
    }
    else if (strncmp (pcParam[1], "vol", LWIP_HTTPD_MAX_TAG_NAME_LEN ) == 0)
    {
      LWIP_DEBUGF(HTTP_CGI_DEBUG, ("volume config :%d %s %d\n", channel, pcValue[0], paramval));
      sequencer.set_track_relative_vol (channel, paramval);
    }
  }
  return ("/config/data/empty.json");
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