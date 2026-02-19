namespace GOTHIC_NAMESPACE
{
	namespace Options
	{

	}

	namespace Options::Internals
	{
		GameSub addTrivia(ZTEST(GameEvent::Execute), []
		{

		});

		GameSub load(ZTEST(GameEvent::DefineExternals), []
		{
			ActiveOptionBase::LoadAll();
		});
	}
}