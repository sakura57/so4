using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataEditor
{
    interface SubEditor
    {
        void createForm();

        void closeForm();

        string getName();
    }
}
