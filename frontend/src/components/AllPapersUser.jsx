import { useState, useEffect } from 'react';
import axios from 'axios';
import { toast } from 'react-toastify';
const AllPapersUser = () => {
    const [papers, setPapers] = useState([]);

  const splitPaperDescription = (description) => {
    const text = String(description || "");
    const parts = text.split(/\n\nAuthors:\s*/i);
    return {
      authors: (parts[1] || "").trim(),
    };
  };

  useEffect(() => {
    // Fetch all papers when the component mounts
    const fetchPapers = async () => {
      try {
        const response = await axios.get(`${import.meta.env.VITE_API_URL}/api/v1/notifications`, {
          params: { type: "HIGHLIGHTS", page: 1, limit: 1000 },
        });
        setPapers(response?.data?.data ?? []);
      } catch (error) {
        console.error('Error fetching papers:', error);
        toast.error('Failed to fetch papers. Please try again.');
      }
    };
    fetchPapers();
  }, []);
  return (
    <div>
      <div className="site-card p-6 flex-1 h-[60%] overflow-hidden">
          <h2 className="text-2xl font-semibold text-zinc-900 dark:text-slate-50 mb-4 border-b border-zinc-200 dark:border-slate-700/60 pb-2">📄 Papers Received</h2>
          <ul className="space-y-3 max-h-[500px] overflow-y-auto">
            {papers.map((paper, index) => (
              <li key={index} className="rounded-lg border border-zinc-200 bg-amber-50/80 p-4 shadow-sm flex justify-between items-center transition-transform duration-300 dark:border-slate-700/60 dark:bg-amber-500/10">
                <div>
                  <strong className="text-zinc-900 dark:text-slate-50">{paper.title}</strong>
                  <p className="text-zinc-600 dark:text-slate-300 text-sm">Authors: {splitPaperDescription(paper.description).authors || "—"}</p>
                </div>
                {paper.link && (
                  <a href={paper.link} target="_blank" rel="noopener noreferrer">
                    <button className="btn btn-primary">
                      View
                    </button>
                  </a>
                )}
              </li>
            ))}
          </ul>
        </div>
    </div>
  )
}

export default AllPapersUser
