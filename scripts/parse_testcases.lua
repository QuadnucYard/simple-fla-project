-- Function to split text into lines
local function _split_lines(text)
	local lines = {}
	for line in text:gmatch("[^\n]*") do
		table.insert(lines, line)
	end
	return lines
end

-- Function to parse the text into blocks based on '---' separator
local function _parse_blocks(lines)
	local blocks = {}
	local current_block = {}

	for _, line in ipairs(lines) do
		if line == '---' then
			if #current_block > 0 then
				table.insert(blocks, current_block)
			end
			current_block = {}
		else
			table.insert(current_block, line)
		end
	end

	-- Insert the last block if it exists
	if #current_block > 0 then
		table.insert(blocks, current_block)
	end

	return blocks
end

function main(text)
	-- Split the text into lines
	local lines = _split_lines(text)

	-- Parse the lines into blocks
	local blocks = _parse_blocks(lines)

	-- Create testcases from the parsed blocks
	local testcases = {}
	for _, block in ipairs(blocks) do
		table.insert(testcases, { source = block[1], input = block[2], output = block[3] })
	end
	return testcases
end
