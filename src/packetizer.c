/**
  ******************************************************************************
  * @file    packetizer.c
  * @author  Moldovan Istvan
  * @version V1.0.0
  * @brief   Packetizer on STM32
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

#define ETHARP_SUPPORT_VLAN	1

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "netif/etharp.h"
#include <string.h>
#include <stdio.h>
#include "spi_pov.h"

/* Private typedef -----------------------------------------------------------*/

PACK_STRUCT_BEGIN
struct pov_hdr{
  PACK_STRUCT_FIELD(uint32_t  frame_id);
  PACK_STRUCT_FIELD(uint8_t   subframe_no);
  PACK_STRUCT_FIELD(uint8_t   col_per_frame);
  PACK_STRUCT_FIELD(uint8_t   delay_us);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Packet structure and header pointers---------------------------------------*/
static u8_t   data[1500];
static struct eth_hdr *eth;
static struct eth_vlan_hdr *vlan;
static struct ip_hdr *iph;
static struct udp_hdr *udph;
static struct pov_hdr *phdr;
static char *payload;

/* Private variables ---------------------------------------------------------*/
int col=0;
__IO   uint32_t message_count = 0;
__IO   uint8_t  Flag = 0;
struct udp_pcb *upcb;

/* Private function prototypes -----------------------------------------------*/
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/* Private functions ---------------------------------------------------------*/


void udp_packetizer_init(){

  /* Initialize packet header structure  */
  eth = (struct eth_hdr *) data;
  vlan = (struct eth_vlan_hdr*)(data + sizeof(struct eth_hdr));
  iph = (struct ip_hdr *) ((uint8_t*)vlan + sizeof(struct eth_vlan_hdr));
  udph = (struct udp_hdr *) ((uint8_t*)iph + sizeof(struct ip_hdr));
  phdr = (struct pov_hdr *) ((uint8_t*)udph + sizeof(struct udp_hdr));
  payload = (char *)((uint8_t*)phdr + sizeof(struct pov_hdr));

  /* Construct the Ethernet header */

  /* Ethernet header */
  eth->src.addr[0] = MAC_ADDR0;
  eth->src.addr[1] = MAC_ADDR1;
  eth->src.addr[2] = MAC_ADDR2;
  eth->src.addr[3] = MAC_ADDR3;
  eth->src.addr[4] = MAC_ADDR4;
  eth->src.addr[5] = MAC_ADDR5;

  eth->dest.addr[0] = DEST_MAC_ADDR0;
  eth->dest.addr[1] = DEST_MAC_ADDR1;
  eth->dest.addr[2] = DEST_MAC_ADDR2;
  eth->dest.addr[3] = DEST_MAC_ADDR3;
  eth->dest.addr[4] = DEST_MAC_ADDR4;
  eth->dest.addr[5] = DEST_MAC_ADDR5;

  /* Ethertype field */
//	eth->type = htons(0x800);
  eth->type = htons(0x8100);

  /* VLAN header */
  vlan->tpid = htons(0xC041);
  vlan->prio_vid = htons(0x800);

  //Fill in the IP Header
  IPH_VHLTOS_SET(iph, 4, IP_HLEN/4, 0x18);
  //IPH_LEN_SET(iph, htons(sizeof (struct ip_hdr) + sizeof (struct udp_hdr) + payload_len));
  IPH_ID_SET(iph, 1);
  IPH_OFFSET_SET(iph, 0);
  IPH_TTL_SET(iph, 32);
  IPH_PROTO_SET(iph, IP_PROTO_UDP);
  IPH_CHKSUM_SET(iph, 0);
  IP4_ADDR(&iph->src,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP4_ADDR(&iph->dest,DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);

  //Fill in the UDP Header
  udph->src = htons(UDP_CLIENT_PORT);
  udph->dest = htons(UDP_SERVER_PORT);
//  udph->len = htons(8 + payload_len);
  udph->chksum = 0;

  //Fill in the POV Header
//  phdr->col_per_frame=payload_len/12;
  phdr->delay_us=COLUMN_DELAY;

}

/**
  *  @brief Set the frame id for the next subframes
  * @param  frame_id
  * @retval None
  */
void udp_packetizer_set_frame_id(uint32_t frame_id)
{
  phdr->frame_id = frame_id;
  phdr->subframe_no = 0;
}

/**
  *  @brief  Connect to display driver
  * @param  None
  * @retval None
  */
void udp_packetizer_send(uint8_t *d, int payload_len)
{
  struct pbuf *p;
  int tx_len = sizeof(struct eth_hdr)+sizeof(struct eth_vlan_hdr)+sizeof(struct ip_hdr)+sizeof(struct udp_hdr)+sizeof(struct pov_hdr);

  // Update payload dependent header fields
  IPH_LEN_SET(iph, htons(sizeof (struct ip_hdr) + sizeof (struct udp_hdr) + payload_len));
  udph->len = htons(8 + payload_len);
  phdr->col_per_frame=payload_len/12;
  phdr->subframe_no++;

  //Add the payload
  memcpy(payload, d, payload_len);

  // calculate frame length
  tx_len += payload_len;

  p = pbuf_alloc(PBUF_TRANSPORT, tx_len, PBUF_POOL);
  if (p != NULL) {
    /* copy data to pbuf */
    pbuf_take(p, (char*)data, tx_len);

    /* send udp data */
    netif_default->linkoutput(netif_default, p);

    /* free pbuf */
    pbuf_free(p);
  }
  return ;
}


/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
  Flag = 0;
  /*increment message count */
  message_count++;

  /* Free receive pbuf */
  pbuf_free(p);

  /* free the UDP connection, so we can accept new clients */
  udp_remove(upcb);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
