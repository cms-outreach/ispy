#ifndef __IG_OPROF_LISTVIEW_ITEM_H__
#define __IG_OPROF_LISTVIEW_ITEM_H__

class IgOProfListViewItem : public QListViewItem
{
	public:
		QString Name;
		int Hits;
		
		IgOProfListViewItem(QListView *parent, QString &name, int hits)
		:QListViewItem(parent),
		Hits(hits),
		Name(name)
		{
			setText(0,QString::number(Hits, 10));
			setText(1,Name);
		}

		int compare(QListViewItem *i,int col,bool ascending ) const
		{
			IgOProfProcessListViewItem *other = (IgOProfProcessListViewItem *) i;
			switch( col )
			{
				default:	
					return key( col, ascending ).compare( other->key(col, ascending) );
					break;
				case 0:
					if(other->Hits > Hits) return 1;
					else return -1;
					break;
			}
		}

		void hits(int hits) {}
		virtual QString hits(void){return this->text(0);}
		virtual QString name(void){return this->text(1);}
};

#endif
