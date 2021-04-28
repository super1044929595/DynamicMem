
/*******************************************************************************************
       +-----------------------------------------------------------------+
       |  Flash handle by user                       |
       +---------------------------------------+-----------+-------------+
       |  Data:huajinwen 2021-3-31                   |   EEPROM  |   Pin       |
       +---------------------------------------+-----------+-------------+
       | .                                     |   E0(GND) |    1  (0V)  |
       | .                                     |   AC0     |    2        |
       | .                                     |   AC1     |    3        |
       | .                                     |   VSS     |    4  (0V)  |
       | SDA                                   |   SDA     |    5        |
       | SCL                                   |   SCL     |    6        |
       | .                                     |   E1(GND) |    7  (0V)  |
       | .                                     |   VDD     |    8 (3.3V) |
       +---------------------------------------+-----------+-------------+

       ************************************************************************************/

void EEPROM_WRITE(uint16_t regaddress,uint8_t *pdata, uint16_t writebytes);

#define FLASH_PAGE_SIZE    (1024U)

static void Flash_Write(uint16_t regaddress,uint8_t *pdata, uint16_t writebytes)
{
	if((pdata==(uint8_t*)NULL)||(writebytes<=0)){
		return ;
	}
	uint16_t paganumbers=(uint16_t)writebytes/FLASH_PAGE_SIZE;
	uint16_t pageoffset= (uint16_t)writebytes%FLASH_PAGE_SIZE;
	uint16_t num=0;
	uint16_t remain=0;
	uint16_t regaddressoffset=0;
	if((regaddress%FLASH_PAGE_SIZE)==0){
	   //regaddress alligment
	   if(paganumbers==0){
	   	    //only handle this page 
	   		EEPROM_WRITE(regaddress,pdata,pageoffset);
	   }else{
	   		num=0;
	   		do{
	   			EEPROM_WRITE((uint16_t)(regaddress+num*FLASH_PAGE_SIZE),(uint8_t*)(pdata+num*FLASH_PAGE_SIZE),FLASH_PAGE_SIZE);
	   			num++;
	   			remain=(uint16_t)(writebytes-num*FLASH_PAGE_SIZE);
	   		}while(num<paganumbers);

	   		if(pageoffset!=0){
	   			EEPROM_WRITE((uint16_t)(regaddress+num*FLASH_PAGE_SIZE),(uint8_t*)(pdata+num*FLASH_PAGE_SIZE),remain);
	   		}	   
	   }

	}else{
			regaddressoffset=(uint16_t)regaddress%FLASH_PAGE_SIZE;
			if(paganumbers==0){
				EEPROM_WRITE(regaddress,pdata,writebytes);
			}else{
				num=0;
				EEPROM_WRITE(regaddress,pdata,(regaddressoffset));
				do{
					EEPROM_WRITE((num*FLASH_PAGE_SIZE+regaddress+regaddressoffset),(uint8_t*)(pdata+num*FLASH_PAGE_SIZE),FLASH_PAGE_SIZE);
					num++;
					remain=(uint16_t)(writebytes-num*FLASH_PAGE_SIZE);
				}while((num<paganumbers));

				if(pageoffset!=0){
	   				EEPROM_WRITE((uint16_t)(num*FLASH_PAGE_SIZE+regaddress+regaddressoffset),(uint8_t*)(pdata+num*FLASH_PAGE_SIZE),remain);
	   			}	
			}
	}
}


/**
  * @brief  Read a buffer of data from the EEPROM.
  * @param  Instance EEPROM instance. Could be only 0.
  * @param  pBuffer  Pointer to the buffer that receives the data read from
  *         the EEPROM.
  * @param  ReadAddr EEPROM's internal address to start reading from.
  * @param  NbrOfBytes  Number of bytes to be read from the EEPROM.
  * @retval BSP status
  */
int32_t BSP_EEPROM_ReadBuffer(uint32_t Instance, uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NbrOfBytes)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= EEPROM_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Eeprom_Drv->Read(Eeprom_CompObj, ReadAddr, pBuffer, NbrOfBytes) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  Instance EEPROM instance. Could be only 0.
  * @param  pBuffer  Pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr EEPROM's internal address to write to.
  * @param  NbrOfBytes  number of bytes to write to the EEPROM.
  * @retval BSP status
  */
int32_t BSP_EEPROM_WriteBuffer(uint32_t Instance, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NbrOfBytes)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t numofpages, numofsingle, count;
  uint8_t *write_buffer;
  uint32_t write_addr, nbr_of_bytes;
  uint32_t  i;

  if(Instance >= EEPROM_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    write_buffer = pBuffer;
    write_addr   = WriteAddr;
    nbr_of_bytes = NbrOfBytes;
    count = EEPROM_PAGESIZE - (write_addr % EEPROM_PAGESIZE);
    numofpages =  nbr_of_bytes / EEPROM_PAGESIZE;
    numofsingle = nbr_of_bytes % EEPROM_PAGESIZE;

    /* If WriteAddr is EEPROM_PAGESIZE aligned */
    if((write_addr % EEPROM_PAGESIZE) == 0U)
    {
      /* If nbr_of_bytes < EEPROM_PAGESIZE */
      if(numofpages == 0U)
      {
        /* Start writing data */
        if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, numofsingle) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
      }
      /* If nbr_of_bytes > EEPROM_PAGESIZE */
      else
      {
        i = 0;
        do
        {
          if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, EEPROM_PAGESIZE) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            write_addr += EEPROM_PAGESIZE;
            write_buffer += EEPROM_PAGESIZE;
            i++;
          }
        }while((i < numofpages) && (ret == BSP_ERROR_NONE));

        if(numofsingle != 0U)
        {
          if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, numofsingle) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }
      }
    }
    /* If WriteAddr is not EEPROM_PAGESIZE aligned */
    else
    {
      /* If nbr_of_bytes < EEPROM_PAGESIZE */
      if(numofpages == 0U)
      {
        /* If the number of data to be written is more than the remaining space
        in the current page: */
        if(nbr_of_bytes > count)
        {
          /* Write the data contained in same page */
          if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, count) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }/* Write the remaining data in the following page */
          else if(EEPROM_WriteBytes(Instance, (uint8_t*)(write_buffer + count), (write_addr + count), nbr_of_bytes - count) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            ret = BSP_ERROR_NONE;
          }
        }
        else if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, numofsingle) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      /* If nbr_of_bytes > EEPROM_PAGESIZE */
      else
      {
        i = 0;
        nbr_of_bytes -= count;
        numofpages =  nbr_of_bytes / EEPROM_PAGESIZE;
        numofsingle = nbr_of_bytes % EEPROM_PAGESIZE;

        if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, count) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          write_addr += count;
          write_buffer += count;
        }

        do
        {
          if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, EEPROM_PAGESIZE) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            write_addr += EEPROM_PAGESIZE;
            write_buffer += EEPROM_PAGESIZE;
            i++;
          }
        }while((i < numofpages) && (ret == BSP_ERROR_NONE));

        if(numofsingle != 0U)
        {
          if(EEPROM_WriteBytes(Instance, write_buffer, write_addr, numofsingle) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }
      }
    }
  }

  /* Return BSP status */
  return ret;
}
























