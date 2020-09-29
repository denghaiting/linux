/****************************************Copyright (c)****************************************
                                                                                                                                             
                                           
                             Galaxywind Network System Co., Ltd.                                                    
                                                                                                                                
                                  http://www.galaxywind.com                                                             
                                                                                                                                             
                                   
 Author:        Y
 Date  :        2019-9-2
 *********************************************************************************************/
#include "stm32f0xx.h"
#include "CMT2300A_spi.h"
#include "CMT2300A_reg.h"
#include "CMT2300A.h"


u8 read_reg(u8 reg_addr)
{
	u8 value;

	CMT2300A_spi_read_reg(reg_addr, &value, 1);

	return value;
}

void write_reg(u8 reg_addr, u8 byte)
{
	u8 value = byte;
	
	CMT2300A_spi_write_reg(reg_addr, &value, 1);
}



/**********************************************************
**Name:     vIntSrcFlagClr
**Function: clear flag
**Input:    none
**Output:   equ CMT23_INT_EN
**     .7      .6     .5        .4       .3       .2       .1       .0
**   SL_TMO  RX_TMO  TX_TMO  PREAM_OK  SYNC_OK  NODE_OK  CRC_OK  PKT_DONE
**********************************************************/
u8 CMT2300A_IntSrcFlagClr(void)
{
	u8 tmp;
	u8 int_clr2 = 0;
	u8 int_clr1 = 0;
	u8 flg = 0;

	tmp = read_reg(eCUS_INT_FLAG);
	if (tmp & CMT2300A_MASK_LBD_FLG) {				//LBD_FLG_Active
		int_clr2 |= CMT2300A_MASK_LBD_CLR;
	}

	if (tmp & CMT2300A_MASK_PREAM_OK_FLG) {			//Preamble Active
		int_clr2 |= CMT2300A_MASK_PREAM_OK_CLR;
		flg |= CMT2300A_MASK_PREAM_OK_EN;
	}
	
	if (tmp & CMT2300A_MASK_SYNC_OK_FLG) {			//Sync Active
		int_clr2 |= CMT2300A_MASK_SYNC_OK_CLR;		
		flg |= CMT2300A_MASK_SYNC_OK_EN;		
	}
	
	if (tmp & CMT2300A_MASK_NODE_OK_FLG)	{		//Node Addr Active
		int_clr2 |= CMT2300A_MASK_NODE_OK_CLR;	
		flg |= CMT2300A_MASK_NODE_OK_EN;
	}
	
	if (tmp & CMT2300A_MASK_CRC_OK_FLG) {			//Crc Pass Active
		int_clr2 |= CMT2300A_MASK_CRC_OK_CLR;
		flg |= CMT2300A_MASK_CRC_OK_EN;
	}
	
	if (tmp & CMT2300A_MASK_PKT_OK_FLG) {			//Rx Done Active
		int_clr2 |= CMT2300A_MASK_PKT_DONE_CLR;
		flg |= CMT2300A_MASK_PKT_DONE_EN;
	}

	if (tmp & CMT2300A_MASK_COL_ERR_FLG) {			//这两个都必须通过RX_DONE清除
		int_clr2 |= CMT2300A_MASK_PKT_DONE_CLR;
	}
	
	if (tmp & CMT2300A_MASK_PKT_ERR_FLG) {
		int_clr2 |= CMT2300A_MASK_PKT_DONE_CLR;
	}

	write_reg(eCUS_INT_CLR2, int_clr2);	//Clear flag

	tmp = read_reg(eCUS_INT_CLR1);
	if (tmp & CMT2300A_MASK_TX_DONE_FLG) {
		int_clr1 |= CMT2300A_MASK_TX_DONE_CLR;
		flg |= CMT2300A_MASK_TX_DONE_EN;
	}	
	
	if (tmp & CMT2300A_MASK_SL_TMO_FLG) {
		int_clr1 |= CMT2300A_MASK_SL_TMO_CLR;
		flg |= CMT2300A_MASK_SL_TMO_EN;
	} 
	
	if (tmp & CMT2300A_MASK_RX_TMO_FLG) {
		int_clr1 |= CMT2300A_MASK_RX_TMO_CLR;
		flg |= CMT2300A_MASK_RX_TMO_EN;
	}	
	
	write_reg(eCUS_INT_CLR1, int_clr1);	//Clear flag 

	return (flg);
}


void CMT2300A_AfterCfg(void)            //call after vCfgBank
{
	u8 tmp;	
	tmp = read_reg(eCUS_MODE_STA);
	tmp |= CMT2300A_MASK_CFG_RETAIN;
	tmp &= (~CMT2300A_MASK_RSTN_IN_EN);			//Disable RstPin	
	write_reg(eCUS_MODE_STA, tmp);

	tmp = 0x00;                    //Disable All Calibration, when no need to use DutyCycleMode
	write_reg(eCUS_SYS2, tmp);

	CMT2300A_IntSrcFlagClr();
}

u8 CMT2300A_get_RSSI_CODE(void)
{
	return read_reg(eCUS_RSSI_CODE);
}

u8 CMT2300A_get_RSSI_DBM(void)
{
	return read_reg(eCUS_RSSI_DBM);
}

void CMT2300A_switch_status(u8 cmd)
{
    write_reg(eCUS_MODE_CTL, cmd);
}

u8 CMT2300A_get_status(void)
{
	return read_reg(eCUS_MODE_STA) & CMT2300A_MASK_CHIP_MODE_STA;
}


void CMT2300A_soft_reset(void)
{
	u8 value = 0xFF;
	
    write_reg(eCUS_SOFTRST, value);
}


void CMT2300A_LFOSC_set(BOOL en)
{
	u8 tmp;
	tmp = read_reg(eCUS_SYS2);
    
	if (en) {
		tmp |= CMT2300A_MASK_LFOSC_RECAL_EN;
		tmp |= CMT2300A_MASK_LFOSC_CAL1_EN;
		tmp |= CMT2300A_MASK_LFOSC_CAL2_EN;
	}
	else {
		tmp &= ~CMT2300A_MASK_LFOSC_RECAL_EN;
		tmp &= ~CMT2300A_MASK_LFOSC_CAL1_EN;
		tmp &= ~CMT2300A_MASK_LFOSC_CAL2_EN;
	}

	write_reg(eCUS_SYS2, tmp);
}




/*! ********************************************************
* @name    CMT2300A_ConfigGpio
* @desc    Config GPIO pins mode.
* @param   nGpioSel: GPIO1_SEL | GPIO2_SEL | GPIO3_SEL | GPIO4_SEL
*          GPIO1_SEL:
*            CMT2300A_GPIO1_SEL_DOUT/DIN 
*            CMT2300A_GPIO1_SEL_INT1
*            CMT2300A_GPIO1_SEL_INT2 
*            CMT2300A_GPIO1_SEL_DCLK
*
*          GPIO2_SEL:
*            CMT2300A_GPIO2_SEL_INT1 
*            CMT2300A_GPIO2_SEL_INT2
*            CMT2300A_GPIO2_SEL_DOUT/DIN 
*            CMT2300A_GPIO2_SEL_DCLK
*
*          GPIO3_SEL:
*            CMT2300A_GPIO3_SEL_CLKO 
*            CMT2300A_GPIO3_SEL_DOUT/DIN
*            CMT2300A_GPIO3_SEL_INT2 
*            CMT2300A_GPIO3_SEL_DCLK
*
*          GPIO4_SEL:
*            CMT2300A_GPIO4_SEL_RSTIN 
*            CMT2300A_GPIO4_SEL_INT1
*            CMT2300A_GPIO4_SEL_DOUT 
*            CMT2300A_GPIO4_SEL_DCLK
* *********************************************************/
void CMT2300A_ConfigGpio(u8 nGpioSel)
{
    write_reg(eCUS_IO_SEL, nGpioSel);
}


/*! ********************************************************
* @name    CMT2300A_ConfigInterrupt
* @desc    Config interrupt on INT1 and INT2.
* @param   nInt1Sel, nInt2Sel
*            CMT2300A_INT_SEL_RX_ACTIVE
*            CMT2300A_INT_SEL_TX_ACTIVE
*            CMT2300A_INT_SEL_RSSI_VLD
*            CMT2300A_INT_SEL_PREAM_OK
*            CMT2300A_INT_SEL_SYNC_OK
*            CMT2300A_INT_SEL_NODE_OK
*            CMT2300A_INT_SEL_CRC_OK
*            CMT2300A_INT_SEL_PKT_OK
*            CMT2300A_INT_SEL_SL_TMO
*            CMT2300A_INT_SEL_RX_TMO
*            CMT2300A_INT_SEL_TX_DONE
*            CMT2300A_INT_SEL_RX_FIFO_NMTY
*            CMT2300A_INT_SEL_RX_FIFO_TH
*            CMT2300A_INT_SEL_RX_FIFO_FULL
*            CMT2300A_INT_SEL_RX_FIFO_WBYTE
*            CMT2300A_INT_SEL_RX_FIFO_OVF
*            CMT2300A_INT_SEL_TX_FIFO_NMTY
*            CMT2300A_INT_SEL_TX_FIFO_TH
*            CMT2300A_INT_SEL_TX_FIFO_FULL
*            CMT2300A_INT_SEL_STATE_IS_STBY
*            CMT2300A_INT_SEL_STATE_IS_FS
*            CMT2300A_INT_SEL_STATE_IS_RX
*            CMT2300A_INT_SEL_STATE_IS_TX
*            CMT2300A_INT_SEL_LED
*            CMT2300A_INT_SEL_TRX_ACTIVE
*            CMT2300A_INT_SEL_PKT_DONE
* *********************************************************/
void CMT2300A_ConfigInterrupt(u8 nInt1Sel, u8 nInt2Sel)
{
    nInt1Sel &= CMT2300A_MASK_INT1_SEL;
    nInt1Sel |= (~CMT2300A_MASK_INT1_SEL) & read_reg(eCUS_INT1_CTL);
    write_reg(eCUS_INT1_CTL, nInt1Sel);

    nInt2Sel &= CMT2300A_MASK_INT2_SEL;
    nInt2Sel |= (~CMT2300A_MASK_INT2_SEL) & read_reg(eCUS_INT2_CTL);
    write_reg(eCUS_INT2_CTL, nInt2Sel);
}

/*! ********************************************************
* @name    CMT2300A_EnableInterrupt
* @desc    Enable interrupt.
* @param   nEnable 
*            CMT2300A_MASK_SL_TMO_EN   |
*            CMT2300A_MASK_RX_TMO_EN   |
*            CMT2300A_MASK_TX_DONE_EN  |
*            CMT2300A_MASK_PREAM_OK_EN |
*            CMT2300A_MASK_SYNC_OK_EN  |
*            CMT2300A_MASK_NODE_OK_EN  |
*            CMT2300A_MASK_CRC_OK_EN   |
*            CMT2300A_MASK_PKT_DONE_EN
* *********************************************************/
void CMT2300A_EnableInterrupt(u8 nEnable)
{
    write_reg(eCUS_INT_EN, nEnable);
}


/*! ********************************************************
* @name    CMT2300A_EnableAntennaSwitch
* @desc    Enable antenna switch, output TX_ACTIVE/RX_ACTIVE
*          via GPIO1/GPIO2.
* @param   nMode 
*            0: RF_SWT1_EN=1, RF_SWT2_EN=0
*               GPIO1: RX_ACTIVE, GPIO2: TX_ACTIVE
*            1: RF_SWT1_EN=0, RF_SWT2_EN=1
*               GPIO1: RX_ACTIVE, GPIO2: ~RX_ACTIVE
* *********************************************************/
void CMT2300A_EnableAntennaSwitch(u8 nMode)
{
    u8 tmp = read_reg(eCUS_INT1_CTL);

    if (0 == nMode) {
        tmp |= CMT2300A_MASK_RF_SWT1_EN;
        tmp &= ~CMT2300A_MASK_RF_SWT2_EN;
    }
    else if (1 == nMode) {
        tmp &= ~CMT2300A_MASK_RF_SWT1_EN;
        tmp |= CMT2300A_MASK_RF_SWT2_EN;
    }

    write_reg(eCUS_INT1_CTL, tmp);
}



/*! ********************************************************
* @name    CMT2300A_ClearInterruptFlags
* @desc    Clear all interrupt flags.
* @return  Some interrupt flags
*            CMT2300A_MASK_SL_TMO_EN    |
*            CMT2300A_MASK_RX_TMO_EN    |
*            CMT2300A_MASK_TX_DONE_EN   |
*            CMT2300A_MASK_PREAM_OK_FLG |
*            CMT2300A_MASK_SYNC_OK_FLG  |
*            CMT2300A_MASK_NODE_OK_FLG  |
*            CMT2300A_MASK_CRC_OK_FLG   |
*            CMT2300A_MASK_PKT_OK_FLG
* *********************************************************/
u8 CMT2300A_clear_it_flags(void)
{
	u8 nFlag1, nFlag2;
	u8 nClr1 = 0;
	u8 nClr2 = 0;
	u8 nRet  = 0;
	u8 nIntPolar;

	nIntPolar = read_reg(eCUS_INT1_CTL);
	nIntPolar = (nIntPolar & CMT2300A_MASK_INT_POLAR) ? 1 : 0;

	nFlag1 = read_reg(eCUS_INT_FLAG);
	nFlag2 = read_reg(eCUS_INT_CLR1);
    
    if (nIntPolar) {
        /* Interrupt flag active-low */
        nFlag1 = ~nFlag1;
        nFlag2 = ~nFlag2;
    }

    if (CMT2300A_MASK_LBD_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_LBD_CLR;         /* Clear LBD_FLG */
    }

    if (CMT2300A_MASK_COL_ERR_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_PKT_DONE_CLR;    /* Clear COL_ERR_FLG by PKT_DONE_CLR */
    }

    if (CMT2300A_MASK_PKT_ERR_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_PKT_DONE_CLR;    /* Clear PKT_ERR_FLG by PKT_DONE_CLR */
    }

    if (CMT2300A_MASK_PREAM_OK_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_PREAM_OK_CLR;    /* Clear PREAM_OK_FLG */
        nRet  |= CMT2300A_MASK_PREAM_OK_FLG;    /* Return PREAM_OK_FLG */
    }

    if (CMT2300A_MASK_SYNC_OK_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_SYNC_OK_CLR;    /* Clear SYNC_OK_FLG */
        nRet  |= CMT2300A_MASK_SYNC_OK_FLG;    /* Return SYNC_OK_FLG */
    }

	if (CMT2300A_MASK_NODE_OK_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_NODE_OK_CLR;    /* Clear NODE_OK_FLG */
        nRet  |= CMT2300A_MASK_NODE_OK_FLG;    /* Return NODE_OK_FLG */
    }

    if (CMT2300A_MASK_CRC_OK_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_CRC_OK_CLR;    /* Clear CRC_OK_FLG */
        nRet  |= CMT2300A_MASK_CRC_OK_FLG;    /* Return CRC_OK_FLG */
    }

    if (CMT2300A_MASK_PKT_OK_FLG & nFlag1) {
        nClr2 |= CMT2300A_MASK_PKT_DONE_CLR;  /* Clear PKT_OK_FLG */
        nRet  |= CMT2300A_MASK_PKT_OK_FLG;    /* Return PKT_OK_FLG */
    }    

    if (CMT2300A_MASK_SL_TMO_FLG & nFlag2) {
        nClr1 |= CMT2300A_MASK_SL_TMO_CLR;    /* Clear SL_TMO_FLG */
        nRet  |= CMT2300A_MASK_SL_TMO_EN;     /* Return SL_TMO_FLG by SL_TMO_EN */
    }

    if (CMT2300A_MASK_RX_TMO_FLG & nFlag2) {
        nClr1 |= CMT2300A_MASK_RX_TMO_CLR;    /* Clear RX_TMO_FLG */
        nRet  |= CMT2300A_MASK_RX_TMO_EN;     /* Return RX_TMO_FLG by RX_TMO_EN */
    }

    if (CMT2300A_MASK_TX_DONE_FLG & nFlag2) {
        nClr1 |= CMT2300A_MASK_TX_DONE_CLR;   /* Clear TX_DONE_FLG */
        nRet  |= CMT2300A_MASK_TX_DONE_EN;    /* Return TX_DONE_FLG by TX_DONE_EN */
    }
    
    write_reg(eCUS_INT_CLR1, nClr1);
    write_reg(eCUS_INT_CLR2, nClr2);

    if (nIntPolar) {
        /* Interrupt flag active-low */
        nRet = ~nRet;
    }

    return nRet;
}

/*! ********************************************************
* @name    CMT2300A_EnableReadFifo
* @desc    Enable SPI to read the FIFO.
* *********************************************************/
void CMT2300A_EnableReadFifo(void)
{
    u8 tmp = read_reg(eCUS_FIFO_CTL);
    tmp &= ~CMT2300A_MASK_SPI_FIFO_RD_WR_SEL; 
    tmp &= ~CMT2300A_MASK_FIFO_RX_TX_SEL;
    write_reg(eCUS_FIFO_CTL, tmp);
}


/* 合并成 1个 64-byte 的 FIFO */
void CMT2300A_fifo_merge_enable(void)
{
    u8 tmp = read_reg(eCUS_FIFO_CTL);
    tmp |= CMT2300A_MASK_FIFO_MERGE_EN; 
    write_reg(eCUS_FIFO_CTL, tmp);
}

/* 分开为 2个 32-byte 的 FIFO */
void CMT2300A_fifo_merge_disable(void)
{
    u8 tmp = read_reg(eCUS_FIFO_CTL);
    tmp &= ~CMT2300A_MASK_FIFO_MERGE_EN; 
    write_reg(eCUS_FIFO_CTL, tmp);
}



/*! ********************************************************
* @name    CMT2300A_EnableWriteFifo
* @desc    Enable SPI to write the FIFO.
* *********************************************************/
void CMT2300A_EnableWriteFifo(void)
{
    u8 tmp = read_reg(eCUS_FIFO_CTL);
    tmp |= CMT2300A_MASK_SPI_FIFO_RD_WR_SEL;
    tmp |= CMT2300A_MASK_FIFO_RX_TX_SEL;
    write_reg(eCUS_FIFO_CTL, tmp);
}




/*! ********************************************************
* @name    CMT2300A_ClearFifo
* @desc    Clear the Rx FIFO.
* @return  FIFO flags
*            CMT2300A_MASK_RX_FIFO_FULL_FLG |
*            CMT2300A_MASK_RX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_RX_FIFO_TH_FLG   |
*            CMT2300A_MASK_RX_FIFO_OVF_FLG  |
*            CMT2300A_MASK_TX_FIFO_FULL_FLG |
*            CMT2300A_MASK_TX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_TX_FIFO_TH_FLG
* *********************************************************/
u8 CMT2300A_ClearRxFifo(void)
{
    u8 tmp = read_reg(eCUS_FIFO_FLAG);
    write_reg(eCUS_FIFO_CLR, CMT2300A_MASK_FIFO_CLR_RX);
    return tmp;
}

/*! ********************************************************
* @name    CMT2300A_ClearFifo
* @desc    Clear the Tx FIFO.
* @return  FIFO flags
*            CMT2300A_MASK_RX_FIFO_FULL_FLG |
*            CMT2300A_MASK_RX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_RX_FIFO_TH_FLG   |
*            CMT2300A_MASK_RX_FIFO_OVF_FLG  |
*            CMT2300A_MASK_TX_FIFO_FULL_FLG |
*            CMT2300A_MASK_TX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_TX_FIFO_TH_FLG
* *********************************************************/
u8 CMT2300A_ClearTxFifo(void)
{
    u8 tmp = read_reg(eCUS_FIFO_FLAG);
    write_reg(eCUS_FIFO_CLR, CMT2300A_MASK_FIFO_CLR_TX);
    return tmp;
}


