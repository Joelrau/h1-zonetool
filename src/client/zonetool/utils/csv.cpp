#include <std_include.hpp>
#include "csv.hpp"

#ifdef __cplusplus
extern "C" {
#endif

CsvParser* CsvParser_new(const char* filePath, const char* delimiter, int firstLineIsHeader)
{
	CsvParser* csvParser = (CsvParser*)malloc(sizeof(CsvParser));
	if (filePath == nullptr)
	{
		csvParser->filePath_ = nullptr;
	}
	else
	{
		int filePathLen = static_cast<int>(strlen(filePath));
		csvParser->filePath_ = (char*)malloc((filePathLen + 1));
		strcpy(csvParser->filePath_, filePath);
	}
	csvParser->firstLineIsHeader_ = firstLineIsHeader;
	csvParser->errMsg_ = nullptr;
	if (delimiter == nullptr)
	{
		csvParser->delimiter_ = ',';
	}
	else if (_CsvParser_delimiterIsAccepted(delimiter))
	{
		csvParser->delimiter_ = *delimiter;
	}
	else
	{
		csvParser->delimiter_ = '\0';
	}
	csvParser->header_ = nullptr;
	csvParser->fileHandler_ = nullptr;
	csvParser->fromString_ = 0;
	csvParser->csvString_ = nullptr;
	csvParser->csvStringIter_ = 0;

	return csvParser;
}

CsvParser* CsvParser_new_from_string(const char* csvString, const char* delimiter, int firstLineIsHeader)
{
	CsvParser* csvParser = CsvParser_new(nullptr, delimiter, firstLineIsHeader);
	csvParser->fromString_ = 1;
	if (csvString != nullptr)
	{
		int csvStringLen = static_cast<int>(strlen(csvString));
		csvParser->csvString_ = (char*)malloc(csvStringLen + 1);
		strcpy(csvParser->csvString_, csvString);
	}
	return csvParser;
}

void CsvParser_destroy(CsvParser* csvParser)
{
	if (csvParser == nullptr)
	{
		return;
	}
	if (csvParser->filePath_ != nullptr)
	{
		free(csvParser->filePath_);
	}
	if (csvParser->errMsg_ != nullptr)
	{
		free(csvParser->errMsg_);
	}
	if (csvParser->fileHandler_ != nullptr)
	{
		fclose(csvParser->fileHandler_);
	}
	if (csvParser->header_ != nullptr)
	{
		CsvParser_destroy_row(csvParser->header_);
	}
	if (csvParser->csvString_ != nullptr)
	{
		free(csvParser->csvString_);
	}
	free(csvParser);
}

void CsvParser_destroy_row(CsvRow* csvRow)
{
	int i;
	for (i = 0; i < csvRow->numOfFields_; i++)
	{
		free(csvRow->fields_[i]);
	}
	free(csvRow->fields_);
	free(csvRow);
}

const CsvRow* CsvParser_getHeader(CsvParser* csvParser)
{
	if (!csvParser->firstLineIsHeader_)
	{
		_CsvParser_setErrorMessage(
			csvParser, "Cannot supply header, as current CsvParser object does not support header");
		return nullptr;
	}
	if (csvParser->header_ == nullptr)
	{
		csvParser->header_ = _CsvParser_getRow(csvParser);
	}
	return csvParser->header_;
}

CsvRow* CsvParser_getRow(CsvParser* csvParser)
{
	if (csvParser->firstLineIsHeader_ && csvParser->header_ == nullptr)
	{
		csvParser->header_ = _CsvParser_getRow(csvParser);
	}
	return _CsvParser_getRow(csvParser);
}

int CsvParser_getNumFields(const CsvRow* csvRow)
{
	return csvRow->numOfFields_;
}

const char** CsvParser_getFields(const CsvRow* csvRow)
{
	return (const char**)csvRow->fields_;
}

CsvRow* _CsvParser_getRow(CsvParser* csvParser)
{
	int numRowRealloc = 0;
	int acceptedFields = 64;
	int acceptedCharsInField = 64;
	if (csvParser->filePath_ == nullptr && (!csvParser->fromString_))
	{
		_CsvParser_setErrorMessage(csvParser, "Supplied CSV file path is NULL");
		return nullptr;
	}
	if (csvParser->csvString_ == nullptr && csvParser->fromString_)
	{
		_CsvParser_setErrorMessage(csvParser, "Supplied CSV string is NULL");
		return nullptr;
	}
	if (csvParser->delimiter_ == '\0')
	{
		_CsvParser_setErrorMessage(csvParser, "Supplied delimiter is not supported");
		return nullptr;
	}
	if (!csvParser->fromString_)
	{
		if (csvParser->fileHandler_ == nullptr)
		{
			csvParser->fileHandler_ = fopen(csvParser->filePath_, "r");
			if (csvParser->fileHandler_ == nullptr)
			{
				int errorNum = errno;
				const char* errStr = strerror(errorNum);
				char* errMsg = (char*)malloc(1024 + strlen(errStr));
				strcpy(errMsg, "");
				sprintf(errMsg, "Error opening CSV file for reading: %s : %s", csvParser->filePath_, errStr);
				_CsvParser_setErrorMessage(csvParser, errMsg);
				free(errMsg);
				return nullptr;
			}
		}
	}
	CsvRow* csvRow = (CsvRow*)malloc(sizeof(CsvRow));
	csvRow->fields_ = (char**)malloc(acceptedFields * sizeof(char*));
	csvRow->numOfFields_ = 0;
	int fieldIter = 0;
	char* currField = (char*)malloc(acceptedCharsInField);
	int inside_complex_field = 0;
	int currFieldCharIter = 0;
	int seriesOfQuotesLength = 0;
	int lastCharIsQuote = 0;
	int isEndOfFile = 0;
	while (true)
	{
		char currChar = static_cast<char>((csvParser->fromString_)
			                ? csvParser->csvString_[csvParser->csvStringIter_]
			                : fgetc(csvParser->fileHandler_));
		csvParser->csvStringIter_++;
		int endOfFileIndicator;
		if (csvParser->fromString_)
		{
			endOfFileIndicator = (currChar == '\0');
		}
		else
		{
			endOfFileIndicator = feof(csvParser->fileHandler_);
		}
		if (endOfFileIndicator)
		{
			if (currFieldCharIter == 0 && fieldIter == 0)
			{
				_CsvParser_setErrorMessage(csvParser, "Reached EOF");
				free(currField);
				CsvParser_destroy_row(csvRow);
				return nullptr;
			}
			currChar = '\n';
			isEndOfFile = 1;
		}
		if (currChar == '\r')
		{
			continue;
		}
		if (currFieldCharIter == 0 && !lastCharIsQuote)
		{
			if (currChar == '\"')
			{
				inside_complex_field = 1;
				lastCharIsQuote = 1;
				continue;
			}
		}
		else if (currChar == '\"')
		{
			seriesOfQuotesLength++;
			inside_complex_field = (seriesOfQuotesLength % 2 == 0);
			if (inside_complex_field)
			{
				currFieldCharIter--;
			}
		}
		else
		{
			seriesOfQuotesLength = 0;
		}
		if (isEndOfFile || ((currChar == csvParser->delimiter_ || currChar == '\n') && !inside_complex_field))
		{
			currField[lastCharIsQuote ? currFieldCharIter - 1 : currFieldCharIter] = '\0';
			csvRow->fields_[fieldIter] = (char*)malloc(currFieldCharIter + 1);
			strcpy(csvRow->fields_[fieldIter], currField);
			free(currField);
			csvRow->numOfFields_++;
			if (currChar == '\n')
			{
				return csvRow;
			}
			if (csvRow->numOfFields_ != 0 && csvRow->numOfFields_ % acceptedFields == 0)
			{
				csvRow->fields_ = (char**)realloc(csvRow->fields_,
				                                  ((numRowRealloc + 2) * acceptedFields) * sizeof(char*));
				numRowRealloc++;
			}
			acceptedCharsInField = 64;
			currField = (char*)malloc(acceptedCharsInField);
			currFieldCharIter = 0;
			fieldIter++;
			inside_complex_field = 0;
		}
		else
		{
			currField[currFieldCharIter] = currChar;
			currFieldCharIter++;
			if (currFieldCharIter == acceptedCharsInField - 1)
			{
				acceptedCharsInField *= 2;
				currField = (char*)realloc(currField, acceptedCharsInField);
			}
		}
		lastCharIsQuote = (currChar == '\"') ? 1 : 0;
	}
}

int _CsvParser_delimiterIsAccepted(const char* delimiter)
{
	char actualDelimiter = *delimiter;
	if (actualDelimiter == '\n' || actualDelimiter == '\r' || actualDelimiter == '\0' ||
		actualDelimiter == '\"')
	{
		return 0;
	}
	return 1;
}

void _CsvParser_setErrorMessage(CsvParser* csvParser, const char* errorMessage)
{
	if (csvParser->errMsg_ != nullptr)
	{
		free(csvParser->errMsg_);
	}
	int errMsgLen = static_cast<int>(strlen(errorMessage));
	csvParser->errMsg_ = (char*)malloc(errMsgLen + 1);
	strcpy(csvParser->errMsg_, errorMessage);
}

const char* CsvParser_getErrorMessage(CsvParser* csvParser)
{
	return csvParser->errMsg_;
}

#ifdef __cplusplus
}
#endif
