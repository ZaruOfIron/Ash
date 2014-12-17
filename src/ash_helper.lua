module('ash_helper', package.seeall)

all_zero_user = { index = 0, correct = 0, wrong = 0, score = 0 }

-- �����̏��Ԃ�index�ŁAuser button���쐬����
function create_user_buttons(...)
	captions = { ... }
	for index, caption in pairs(captions) do
		ash.config.create_user_button(index, caption)
	end
end

-- �����̏��Ԃ�index�ŁAsystem button���쐬����
function create_system_buttons(...)
	captions = { ... }
	for index, caption in pairs(captions) do
		ash.config.create_system_button(index, caption)
	end
end

-- array�̂Ȃ�����target��T���A����index��Ԃ�
-- ������Ȃ����nil��Ԃ�
function search_array(array, target)
	for idx, value in pairs(array) do
		if value == target then return idx end
	end
	return nil
end

-- user��S���擾����
function get_all_users(answer)
	local users = {}
	for i = 1, answer do
		table.insert(users, ash.get_user(i))
	end

	return users
end

-- �N�C�Y�̋K���ɏ]���ă\�[�g
-- score�������A�������������A�듚�������Ȃ��Aindex���������ق�
function sort_users(users)
	table.sort(users,
		function(a, b)
			if a.score ~= b.score then return a.score > b.score end
			if a.correct ~= b.correct then return a.correct > b.correct end
			if a.wrong ~= b.wrong then return a.wrong < b.wrong end
			return a.index < b.index
		end)
end

-- Finish�̏���
function finish(answer, winner)
	local users = ash_helper.get_all_users(answer)
	ash_helper.sort_users(users)
	for i = 1, winner do
		ash.set_user(users[i].index, {}, {1})
	end
end

-- serialize
function value2str(v)
	local vt = type(v)

	local conv_table = {
		'nil' = function(v) return 'nil' end,
		number = function(v) return string.format('%d', v) end,
		string = function(v) return string.format('"%s"', v) end,
		boolean = function(v) return (v == true and 'true' or 'false') end,
		'function' = function(v) return '"*function"' end,
		thread = function(v) return '"thread"' end,
		userdata = function(v) return '"*userdata"' end
	}

	prod = conv_table[vt]
	return prod == nil and '"UnsupportFormat"' or prod()
end

function field2str(v)
	local vt = type(v)

	local conv_table = {
		number = function(v) return string.format('[%d]', v) end,
		string = function(v) return string.format('%s', v) end
	}

	prod = conv_table[vt]
	return prod == nil and 'UnknownField' or prod()
end

-- �e�[�u���̃V���A���C�Y
function serialize_table(t)
	if not (type(t) == 'table') then return value2str(t) end

	local buf = ''
	local f, v = next(t, nil)
	while f do
		-- ,��t������
		if buf ~= '' then buf = buf .. ',' end
		-- �l
		if type(v) == 'table' then
			buf = buf .. field2str(f) .. '=' .. serialize(v)
		else
			buf = buf .. field2str(f) .. '=' .. value2str(v)
		end
		-- ���̗v�f
		f, v = next(t, f)
	end

	buf = '{' .. buf .. '}'
	return buf
end

-- �f�V���A���C�Y
function deserialize(d)
	return assert(loadstring('return ' .. d))()
end

