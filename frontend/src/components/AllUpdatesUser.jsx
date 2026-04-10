import { useState, useEffect } from 'react';
import axios from 'axios';
import { toast } from 'react-toastify';
const AllUpdatesUser = () => {
    const [updates, setUpdates] = useState([]);

    const stripEventDateLine = (description) => {
      if (!description) return "";
      return String(description)
        .replace(/\n\nEvent Date:.*$/ms, "")
        .trim();
    };
  
    useEffect(() => {
      // Fetch all updates when the component mounts
      const fetchUpdates = async () => {
        try {
          const response = await axios.get(`${import.meta.env.VITE_API_URL}/api/v1/notifications`, {
            params: { type: "RECENT_UPDATES", page: 1, limit: 1000 },
          });
          setUpdates(response?.data?.data ?? []);
        } catch (error) {
          console.error('Error fetching updates:', error);
          toast.error('Failed to fetch updates. Please try again.');
        }
      };
      fetchUpdates();
    }, []);
    return <>
    <div className="site-card p-6 flex-1 overflow-hidden">
          <h2 className="text-2xl font-semibold text-zinc-900 dark:text-slate-50 mb-4 border-b border-zinc-200 dark:border-slate-700/60 pb-2">🔥 Recent Updates</h2>
          <ul className="space-y-3 max-h-[500px] overflow-y-auto">
            {updates.map((update, index) => (
              <li key={index} className="rounded-lg border border-zinc-200 bg-amber-50/80 p-4 shadow-sm flex justify-between items-center transition-transform duration-300 dark:border-slate-700/60 dark:bg-amber-500/10">
                <div>
                  <strong className="text-zinc-900 dark:text-slate-50">{update.title}</strong>
                  <p className="text-zinc-600 dark:text-slate-300 text-sm">{stripEventDateLine(update.description)}</p>
                </div>
                {update.link && (
                  <a href={update.link} target="_blank" rel="noopener noreferrer">
                    <button className="btn btn-primary">
                      View
                    </button>
                  </a>
                )}
              </li>
            ))}
          </ul>
        </div>
    </>
}
export default AllUpdatesUser