function initialize()
	return { answer = 12, winner = 5, title = '1st stage', subtitle = 'ÇTÅõÇQÅ~', quizid = 101, org_user = {correct = 1, wrong = 2, score = 3} }
end

function on_command(index, id)
	ash.set_user(index, {correct = 10, wrong = 20, score = 30}, {100})
end
