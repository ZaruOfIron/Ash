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

