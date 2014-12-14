function initialize()
	return { answer = 12, winner = 5, title = '1st stage', subtitle = '‚T›‚Q~', quizid = 101, org_user = {correct = 1, wrong = 2, score = 3} }
end

function on_command(index, id)
	local user = ash.get_user(index)
	print(string.format("i:%d,c:%d,w:%d,s:%d\n", user.index, user.correct, user.wrong, user.score))
	ash.set_user(user.index, {correct = 30}, {100})
end
