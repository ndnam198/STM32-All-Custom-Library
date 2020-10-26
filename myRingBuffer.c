#include "myRingBuffer.h"

FIFO_Status_t FIFO_Init(FIFO_StructTypedef *FIFO_Struct, uint32_t number_of_element)
{
	FIFO_Struct->Number_of_Element = number_of_element;
	/* number_of_element */
	FIFO_Struct->Size = number_of_element + 1;
	FIFO_Struct->Data_Array = (uint8_t *)calloc(number_of_element, sizeof(uint8_t));
	FIFO_Struct->read_index = 0;
	FIFO_Struct->write_index = 0;
	if (IS_NULL_POINTER(FIFO_Struct->Data_Array))
	{
		return FIFO_FAILED;
	}
	else
	{
		return FIFO_SUCCESS;
	}
}

FIFO_Status_t FIFO_WriteData(FIFO_StructTypedef *FIFO_Struct, uint8_t data)
{
	/**
	 * Short explanation:  
	 * After initialization: read_index = 0 =====> (FIFO_Struct->read_index - 1 + FIFO_Struct->Size) % FIFO_Struct->Size =  9 % 10 = 9
	 * If read 1 element: read_index = 1 =====> (FIFO_Struct->read_index - 1 + FIFO_Struct->Size) % FIFO_Struct->Size = 10 % 10 = 0
	 * If you're about to write data into buffer, but write pointer is currently pointing to behind position of read pointer then considering it as a full buffer condition. Exception happens when buffer is just created and write_index = read_index  
	 * or for short: if read_index - write_index == 1 ===> buffer full 
	 */
	if (FIFO_Struct->write_index == ((FIFO_Struct->read_index - 1 + FIFO_Struct->Size) % FIFO_Struct->Size))
	{
		return FIFO_BUFFER_FULL;
	}
	/* Write data to ring buffer with offset = write_index from beginning */
	*(FIFO_Struct->Data_Array + FIFO_Struct->write_index) = data;
	/* write_index always stays in range of [0: FIFO_Struct->Size - 1] */
	FIFO_Struct->write_index = (FIFO_Struct->write_index + 1) % FIFO_Struct->Size;
	return FIFO_SUCCESS; //OK
}

FIFO_Status_t FIFO_ReadData(FIFO_StructTypedef *FIFO_Struct, uint8_t *data)
{
	if (FIFO_Struct->read_index == FIFO_Struct->write_index)
	{
		return FIFO_BUFFER_EMPTY; // Fifo is empty
	}
	*data = *(FIFO_Struct->Data_Array + FIFO_Struct->read_index);
	/**
	 * If (FIFO_Struct->read_index + 1) overflow buffer size ==> read_index = 0
	 */
	FIFO_Struct->read_index = (FIFO_Struct->read_index + 1) % FIFO_Struct->Size;
	return FIFO_SUCCESS; //OK
}