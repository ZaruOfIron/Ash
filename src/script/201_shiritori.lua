ANSWER = 11
WINNER = 3

function initialize()
	-- create user buttons
	ash.config.create_user_button(1, 'CORRECT')
	ash.config.create_user_button(2, '����Ƃ�')
	ash.config.create_user_button(3, 'WRONG')

	-- create system buttons	
	ash.config.create_system_button(1, 'FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '2nd stage',
		subtitle = '����Ƃ�',
		quizid = 201,
		org_user = {
			correct = 0,
			wrong = 0,
			score = 0
		}
	}
end

function on_command(index, id)
	if index == 0 then	-- system
		if id == 1 then	-- finish
			-- user��S���擾����
			local users = {}
			for i = 1, ANSWER do
				table.insert(users, ash.get_user(i))
			end
			
			-- �N�C�Y�̋K���ɏ]���ă\�[�g
			-- score�������A�������������A�듚�������Ȃ��Aindex���������ق�
			table.sort(users,
				function(a, b)
					if a.score ~= b.score then return a.score > b.score end
					if a.correct ~= b.correct then return a.correct > b.correct end
					if a.wrong ~= b.wrong then return a.wrong < b.wrong end
					return a.index < b.index
				end)

			-- ��������WINNER�͒ʉ�
			for i = 1, WINNER do
				ash.set_user(users[i].index, {}, {1})
			end
		end
	else	-- user
		local user, data, info = ash.get_user(index), {}, {}

		if id == 1 then	-- correct
			data.correct = user.correct + 1
			data.score = user.score + 1
		elseif id == 2 then	-- ����Ƃ�
			data.correct = user.correct + 1
			data.score = user.score + 2
			table.insert(info, 20101)
		elseif id == 3 then	-- wrong
			data.wrong = user.wrong + 1
			data.score = user.score - 1
		end

		ash.set_user(index, data, info)
	end
end

