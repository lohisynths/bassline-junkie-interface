/*
 * EEPROM.h
 *
 *  Created on: Jun 10, 2018
 *      Author: alax
 */

#ifndef SRC_EEPROM_H_
#define SRC_EEPROM_H_

#include "stm32f4xx_hal_flash.h"

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */

#define FLASH_USER_START_ADDR   (ADDR_FLASH_SECTOR_7)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (FLASH_END)   /* End @ of user Flash area */


class EEPROM {
public:
	EEPROM(){};
	virtual ~EEPROM(){};

	void erase(uint32_t size) {
		  /* Unlock the Flash to enable the flash control register access *************/
		  HAL_FLASH_Unlock();

		  /* Erase the user Flash area
		    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		  /* Get the 1st sector to erase */
		  uint32_t FirstSector = GetSector(FLASH_USER_START_ADDR);
		  /* Get the number of sector to erase from 1st sector*/
		  uint32_t NbOfSectors = GetSector(FLASH_USER_START_ADDR+size) - FirstSector + 1;

		  /* Fill EraseInit structure*/
		  FLASH_EraseInitTypeDef EraseInitStruct;
		  EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
		  EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
		  EraseInitStruct.Sector = FirstSector;
		  EraseInitStruct.NbSectors = NbOfSectors;

		  uint32_t SectorError = 0;
		  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
		  {
		    /*
		      Error occurred while sector erase.
		      User can add here some code to deal with this error.
		      SectorError will contain the faulty sector and then to know the code error on this sector,
		      user can call function 'HAL_FLASH_GetError()'
		    */
		    /*
		      FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
		    */
		    Error_Handler();
		  }
		  HAL_FLASH_Lock();
	}


	void erase() {
		  /* Unlock the Flash to enable the flash control register access *************/
		  HAL_FLASH_Unlock();

		  /* Erase the user Flash area
		    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		  /* Get the 1st sector to erase */
		  uint32_t FirstSector = GetSector(FLASH_USER_START_ADDR);
		  /* Get the number of sector to erase from 1st sector*/
		  uint32_t NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;

		  /* Fill EraseInit structure*/
		  FLASH_EraseInitTypeDef EraseInitStruct;
		  EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
		  EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
		  EraseInitStruct.Sector = FirstSector;
		  EraseInitStruct.NbSectors = NbOfSectors;

		  uint32_t SectorError = 0;
		  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
		  {
		    /*
		      Error occurred while sector erase.
		      User can add here some code to deal with this error.
		      SectorError will contain the faulty sector and then to know the code error on this sector,
		      user can call function 'HAL_FLASH_GetError()'
		    */
		    /*
		      FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
		    */
		    Error_Handler();
		  }
		  HAL_FLASH_Lock();
	}

	void read() {
		  /* Check if the programmed data is OK
		      MemoryProgramStatus = 0: data programmed correctly
		      MemoryProgramStatus != 0: number of words not programmed correctly ******/
		  uint32_t Address = FLASH_USER_START_ADDR;

		  while (Address < FLASH_USER_END_ADDR)
		  {
		    uint32_t data32 = *(__IO uint32_t*)Address;

		    Address = Address + 4;
		  }
	}

	void write() {
		  /* Program the user Flash area word by word
		    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
		  uint16_t i=0;
		  HAL_FLASH_Unlock();
		  uint32_t Address = FLASH_USER_START_ADDR;

		  while (Address < FLASH_USER_END_ADDR)
		  {
		    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, Address, i++) == HAL_OK)
		    {
		      Address = Address + 4;
		    }
		    else
		    {
		      /* Error occurred while writing data in Flash memory.
		         User can add here some code to deal with this error */
		      /*
		        FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
		      */
		      Error_Handler();
		    }
		  }
		  /* Lock the Flash to disable the flash control register access (recommended
		     to protect the FLASH memory against possible unwanted operation) *********/
		  HAL_FLASH_Lock();
	}

	void Error_Handler(void)
	{
	  /* Turn LED5 (RED) on */
	  while(1)
	  {
	  }
	}

	uint32_t readEEPROMWord(uint32_t address) {
	    uint32_t val = 0;
	    address = address + FLASH_USER_START_ADDR;
	    val = *(__IO uint32_t*)address;

	    return val;
	}


	void writeEEPROMWord(uint32_t address, uint32_t data) {
		  HAL_FLASH_Unlock();
	      address = address + FLASH_USER_START_ADDR;

	      if(address < FLASH_USER_END_ADDR)
	      {
	    		    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, data) == HAL_OK)
	    		    {
	    		    	;
	    		    }
	    		    else
	    		    {
	    		      /* Error occurred while writing data in Flash memory.
	    		         User can add here some code to deal with this error */
	    		      /*
	    		        FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
	    		      */
	    		      Error_Handler();
	    		    }
	      }

		  HAL_FLASH_Lock();
	}

	/**
	  * @brief  Gets the sector of a given address
	  * @param  None
	  * @retval The sector of a given address
	  */
	static uint32_t GetSector(uint32_t Address)
	{
	  uint32_t sector = 0;

	  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	  {
	    sector = FLASH_SECTOR_0;
	  }
	  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	  {
	    sector = FLASH_SECTOR_1;
	  }
	  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	  {
	    sector = FLASH_SECTOR_2;
	  }
	  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	  {
	    sector = FLASH_SECTOR_3;
	  }
	  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	  {
	    sector = FLASH_SECTOR_4;
	  }
	  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	  {
	    sector = FLASH_SECTOR_5;
	  }
	  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	  {
	    sector = FLASH_SECTOR_6;
	  }
	   else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_6))*/
	  {
	    sector = FLASH_SECTOR_7;
	  }

	  return sector;
	}

};

#endif /* SRC_EEPROM_H_ */
