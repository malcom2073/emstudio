Ths is the code style guide. It will evolve, and I guarantee some of the code in the repo does not match this, but that's ok! I'm working towards it.
Also, yes I know tabs are not commonly preferred, and having brackets on new lines increases file size and line count... but I like both, so please follow this guide if you want your pull requests to be accepted.
If I have to make modifications to your pull to make it follow this guide, it will at best extend the time it will take for the pull to be accepted, and at worst it will prevent me from accepting the pull entirely
## Indentation
Tabs are the standard indentation. Spaces will be used for precise alignment where required.
## Exceptions
Are not to be used, unless a library uses them, in which case they will be kept as close to the library boundry as possible.
## Formatting
Formatting guidelines
### Classes:
Brackets will be on newline
Class names will be CamelCase, whereas function names will be camelCase
Pay attention to the spacing below, as this is the preferred method.
Member variable names start with m_, and follow function naming.

	class MyClass : public QObject
	{
	Q_OBJECT
	public:
		MyClass(QObject *parent=0);
		void setMyName(const QString &name);
	private:
		QString m_myClassName;
		int m_otherVariable;
	}
### Functions:
Switch cases should have brackets around them. Fallthrough is acceptable, but explicit cases and breaks are preferred.

	void MyClass::setMyName(const QString &name)
	{
		switch (m_otherVariable)
		{
			case 1:
			{
				m_myClassName = name;
				break;
			}
			case 2:
			{
				m_myClassName = name.toLower();
				break;
			}
		}
	}
