t1 = 0
t2 = 1

function initialize()
	print 'DEBUG'
	ash.config.add_tracking_var('t1')
	ash.config.add_tracking_var('t2')

	return { answer = 12, winner = 5, title = '1st stage', subtitle = 'ÇTÅõÇQÅ~', quizid = 101, org_user = {correct = 1, wrong = 2, score = 3} }
end

function on_command(index, id)

end
